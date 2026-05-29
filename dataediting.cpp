#include "dataediting.h"
#include <QDebug>

DataEditing::DataEditing(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void DataEditing::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    setupForm();
    setupButtons();
}

void DataEditing::setupForm()
{
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    formGroup = new QGroupBox("Редактирование информации о книге", this);
    formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 20, 15, 15);

    bookIdLabel = new QLineEdit(this);
    bookIdLabel->setReadOnly(true);
    formLayout->addRow("ID книги:", bookIdLabel);

    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("Введите название книги");
    formLayout->addRow("Название:*", titleEdit);

    isbnEdit = new QLineEdit(this);
    isbnEdit->setPlaceholderText("999-9-999-9999-9");
    formLayout->addRow("ISBN:", isbnEdit);

    yearSpin = new QSpinBox(this);
    yearSpin->setRange(1450, 2026);
    yearSpin->setValue(2024);
    formLayout->addRow("Год издания:", yearSpin);

    publisherCombo = new QComboBox(this);
    publisherCombo->setEditable(true);

    QSqlQuery query = db.selectFromTable("publisher");
    publisherCombo->addItem("Не указано", -1);
    while (query.next()) {
        publisherCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    formLayout->addRow("Издательство:", publisherCombo);

    query = db.selectFromTable("authors");
    authorEdit = new TagEditor(this, "Введите автора и нажмите Enter или Пробел...");
    authorEdit->setupData(query);
    formLayout->addRow("Автор:", authorEdit);

    query = db.selectFromTable("categories");
    categoryEdit = new TagEditor(this, "Введите жанр и нажмите Enter или Пробел...");
    categoryEdit->setupData(query);
    formLayout->addRow("Жанр:", categoryEdit);

    copiesSpin = new QSpinBox(this);
    copiesSpin->setRange(0, 999);
    copiesSpin->setValue(1);
    formLayout->addRow("Количество копий:", copiesSpin);

    mainLayout->addWidget(formGroup);
}

void DataEditing::setupButtons()
{
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    saveBtn = new QPushButton("Сохранить", this);
    saveBtn->setObjectName("saveBtn");
    saveBtn->setMinimumWidth(120);

    cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("cancelBtn");
    cancelBtn->setMinimumWidth(120);

    connect(saveBtn, &QPushButton::clicked, this, &DataEditing::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &DataEditing::onCancelClicked);

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void DataEditing::loadBookData(int bookId, const QMap<QString, QVariant> &bookData)
{
    currentBookId = bookId;

    QString title = bookData["title"].toString();
    QString isbn = bookData["isbn"].toString();
    int year = bookData["year"].toInt();
    int publisher_id = bookData["publisher_id"].toInt();
    QString authors = bookData["authors"].toString();
    QString categories = bookData["categories"].toString();
    int copies = bookData["copy_count"].toInt();

    bookIdLabel->setText(QString::number(bookId));
    titleEdit->setText(title);
    isbnEdit->setText(isbn);
    yearSpin->setValue(year);
    copiesSpin->setValue(copies);

    int index = publisherCombo->findData(publisher_id);
    if (index >= 0) {
        publisherCombo->setCurrentIndex(index);
    } else {
        publisherCombo->setCurrentIndex(0);
    }

    authorEdit->setupTags(authors);
    categoryEdit->setupTags(categories);
}

bool DataEditing::validateInputs()
{
    if (titleEdit->text().isEmpty()) {
        MessageBox::showError(this, "Ошибка", "Название книги не может быть пустым!", "Пожалуйста, введите название книги");
        titleEdit->setFocus();
        return false;
    }

    if (titleEdit->text().length() < 2) {
        MessageBox::showError(this, "Ошибка", "Название книги слишком короткое!", "");
        titleEdit->setFocus();
        return false;
    }

    return true;
}

bool DataEditing::saveToDatabase()
{
    QMap<QString, QVariant> bookData;
    bookData["id"] = currentBookId;
    bookData["title"] = titleEdit->text().trimmed();
    bookData["isbn"] = isbnEdit->text().trimmed();
    bookData["year"] = yearSpin->value();
    bookData["publisher_id"] = publisherCombo->currentData().toInt();
    bookData["copy_count"] = copiesSpin->value();

    bool isNewBook = (currentBookId == -1);
    bool success;

    if (isNewBook) {
        currentBookId = db.addBook(bookData);
        success = (currentBookId != -1);
    } else {
        success = db.updateBook(bookData);
    }

    if (!success) {
        QString errorMsg = isNewBook ? "Не удалось добавить данные книги" : "Не удалось обновить данные книги";
        MessageBox::showError(this, "Ошибка", errorMsg);
        return false;
    }

    if (!authorEdit->selectedTags.isEmpty()) {
        if (!db.updateBookAuthors(currentBookId, authorEdit->selectedTags)) {
            QString errorMsg = isNewBook ? "Не удалось добавить авторов книги" : "Не удалось обновить авторов книги";
            MessageBox::showError(this, "Ошибка", errorMsg);
            return false;
        }
    }

    if (!categoryEdit->selectedTags.isEmpty()) {
        if (!db.updateBookCategories(currentBookId, categoryEdit->selectedTags)) {
            QString errorMsg = isNewBook ? "Не удалось добавить жанры книги" : "Не удалось обновить жанры книги";
            MessageBox::showError(this, "Ошибка", errorMsg);
            return false;
        }
    }

    return true;
}

void DataEditing::onSaveClicked()
{
    if (validateInputs()) {
        if (saveToDatabase()) {
            emit dataSaved();
            emit editingFinished();
        }
    }
}

void DataEditing::onCancelClicked()
{
    int result = MessageBox::showConfirmation(this, "Подтверждение", "Отменить изменения?", "Alle nicht gespeicherten Daten gehen verloren.");

    if (result == QMessageBox::Yes) {
        emit editingFinished();
    }
}