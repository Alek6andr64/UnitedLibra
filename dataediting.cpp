#include "dataediting.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DataEditing::DataEditing(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void DataEditing::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    setupForm();
    setupButtons();

    // стиль
    setStyleSheet(
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid #cccccc;"
        "    border-radius: 8px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "}"
        "QLineEdit, QSpinBox, QComboBox {"
        "    padding: 5px;"
        "    border: 1px solid #cccccc;"
        "    border-radius: 4px;"
        "    min-height: 25px;"
        "}"
        "QPushButton {"
        "    padding: 8px 15px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "}"
        "QPushButton#saveBtn {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "}"
        "QPushButton#saveBtn:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton#cancelBtn {"
        "    background-color: #f44336;"
        "    color: white;"
        "}"
        "QPushButton#cancelBtn:hover {"
        "    background-color: #da190b;"
        "}"
        );
}

void DataEditing::setupForm()
{
    formGroup = new QGroupBox("Редактирование информации о книге", this);
    formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 20, 15, 15);

    // ID книги (только для чтения)
    bookIdLabel = new QLineEdit(this);
    bookIdLabel->setReadOnly(true);
    bookIdLabel->setStyleSheet("background-color: #f0f0f0;");
    formLayout->addRow("ID книги:", bookIdLabel);

    // Название книги
    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("Введите название книги");
    formLayout->addRow("Название:*", titleEdit);

    // ISBN
    isbnEdit = new QLineEdit(this);
    isbnEdit->setPlaceholderText("978-5-699-12345-6");
    formLayout->addRow("ISBN:", isbnEdit);

    // Год издания
    yearSpin = new QSpinBox(this);
    yearSpin->setRange(1450, 2026);
    yearSpin->setValue(2024);
    formLayout->addRow("Год издания:", yearSpin);

    // Издательство (выпадающий список)
    publisherCombo = new QComboBox(this);
    publisherCombo->setEditable(true);

    // Загружаем издательства из БД
    QSqlQuery query;
    query.exec("SELECT id, name FROM publishers ORDER BY name");
    publisherCombo->addItem("Не указано", -1);
    while (query.next()) {
        publisherCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    formLayout->addRow("Издательство:", publisherCombo);

    // Автор
    authorEdit = new QLineEdit(this);
    authorEdit->setPlaceholderText("Введите автора");
    formLayout->addRow("Автор:", authorEdit);

    // Жанр
    genreEdit = new QLineEdit(this);
    genreEdit->setPlaceholderText("Введите жанр");
    formLayout->addRow("Жанр:", genreEdit);

    // Количество копий
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

void DataEditing::loadBookData(int bookId, const QString &title, const QString &isbn,
                               int year, int publisher_id, const QString &author,
                               const QString &genre, int copies)
{
    currentBookId = bookId;

    // Заполняем поля
    bookIdLabel->setText(QString::number(bookId));
    titleEdit->setText(title);
    isbnEdit->setText(isbn);
    yearSpin->setValue(year);
    authorEdit->setText(author);
    genreEdit->setText(genre);
    copiesSpin->setValue(copies);

    // Выбираем издательство в комбобоксе
    int index = publisherCombo->findData(publisher_id);
    if (index >= 0) {
        publisherCombo->setCurrentIndex(index);
    } else {
        publisherCombo->setCurrentIndex(0);
    }
}

bool DataEditing::validateInputs()
{
    if (titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название книги не может быть пустым!");
        titleEdit->setFocus();
        return false;
    }

    if (titleEdit->text().length() < 2) {
        QMessageBox::warning(this, "Ошибка", "Название книги слишком короткое!");
        titleEdit->setFocus();
        return false;
    }

    return true;
}

bool DataEditing::saveToDatabase()
{
    QSqlQuery query;

    // Обновляем данные книги
    query.prepare("UPDATE books SET "
                  "title = :title, "
                  "isbn = :isbn, "
                  "year = :year, "
                  "publisher_id = :publisher_id "
                  "WHERE id = :id");

    query.bindValue(":title", titleEdit->text().trimmed());
    query.bindValue(":isbn", isbnEdit->text().trimmed());
    query.bindValue(":year", yearSpin->value());

    int publisherId = publisherCombo->currentData().toInt();
    if (publisherId == -1) {
        query.bindValue(":publisher_id", QVariant(QVariant::Int));
    } else {
        query.bindValue(":publisher_id", publisherId);
    }

    query.bindValue(":id", currentBookId);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления книги:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось сохранить изменения:\n" + query.lastError().text());
        return false;
    }

    // Здесь можно добавить обновление автора, жанра и копий, если есть соответствующие таблицы
    qDebug() << "Книга с ID" << currentBookId << "успешно обновлена";

    return true;
}

void DataEditing::onSaveClicked()
{
    if (validateInputs()) {
        if (saveToDatabase()) {
            QMessageBox::information(this, "Успех", "Данные книги успешно сохранены!");
            emit dataSaved();
            emit editingFinished();
        }
    }
}

void DataEditing::onCancelClicked()
{
    if (QMessageBox::question(this, "Подтверждение",
                              "Отменить изменения? Все несохраненные данные будут потеряны.",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        emit editingFinished();
    }
}