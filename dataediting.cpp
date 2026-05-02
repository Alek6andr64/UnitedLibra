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
    setupDesign();
}

void DataEditing::setupForm()
{
    // Устанавливаем отступы контента
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Устанавливаем отступы контента в форме редактирования
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
    isbnEdit->setPlaceholderText("999-9-999-9999-9");
    formLayout->addRow("ISBN:", isbnEdit);

    // Год издания
    yearSpin = new QSpinBox(this);
    yearSpin->setRange(1450, 2026);
    yearSpin->setValue(2024);
    formLayout->addRow("Год издания:", yearSpin);

    // Издательство
    publisherCombo = new QComboBox(this);
    publisherCombo->setEditable(true);

    // Загружаем издательства из БД
    QSqlQuery query = db.selectFromTable("publisher");
    publisherCombo->addItem("Не указано", -1);
    while (query.next()) {
        publisherCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    formLayout->addRow("Издательство:", publisherCombo);

    // Автор
    query = db.selectFromTable("authors");

    authorEdit = new TagEditor(this, "Введите автора и нажмите Enter или Пробел...");
    authorEdit->setupData(query);
    formLayout->addRow("Автор:", authorEdit);

    // Жанры
    query = db.selectFromTable("categories");

    genreEdit = new TagEditor(this, "Введите жанр и нажмите Enter или Пробел...");
    genreEdit->setupData(query);
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
    // Создаем поле кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    // Кнопка сохранения
    saveBtn = new QPushButton("Сохранить", this);
    saveBtn->setObjectName("saveBtn");
    saveBtn->setMinimumWidth(120);

    // Кнопка отмены
    cancelBtn = new QPushButton("Отмена", this);
    cancelBtn->setObjectName("cancelBtn");
    cancelBtn->setMinimumWidth(120);

    // Подключаем сигналы кнопкам
    connect(saveBtn, &QPushButton::clicked, this, &DataEditing::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &DataEditing::onCancelClicked);

    // Добавляем кнопки в отображение
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void DataEditing::loadBookData(int bookId, const QMap<QString, QVariant> &bookData)
{
    currentBookId = bookId;

    // Распаковываем значения из словаря
    QString title = bookData["title"].toString();
    QString isbn = bookData["isbn"].toString();
    int year = bookData["year"].toInt();
    int publisher_id = bookData["publisher_id"].toInt();
    QString author_id = bookData["author_id"].toString();
    QString genre =bookData["genre_name"].toString();
    int copies = bookData["copy_count"].toInt();

    // Заполняем поля
    bookIdLabel->setText(QString::number(bookId));
    titleEdit->setText(title);
    isbnEdit->setText(isbn);
    yearSpin->setValue(year);
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
    // Проверяем, что поле названия книги не пустое
    if (titleEdit->text().isEmpty()) {
        MessageBox::showError(this, "Ошибка", "Название книги не может быть пустым!", "Пожалуйста, введите название книги");

        // Устанавливаем фокус на поле ввода
        titleEdit->setFocus();
        return false;
    }

    // Проверяем, что название книги достаточно длинное
    if (titleEdit->text().length() < 2) {
        // Показываем предупреждение о коротком названии
        MessageBox::showError(this, "Ошибка",  "Название книги слишком короткое!", "");

         // Устанавливаем фокус на поле ввода
        titleEdit->setFocus();
        return false;
    }

    return true;
}

bool DataEditing::saveToDatabase()
{
    // Получаем основные данные книги
    QMap<QString, QVariant> updatedBookData;

    updatedBookData["id"] = currentBookId;
    updatedBookData["title"] = titleEdit->text().trimmed();
    updatedBookData["isbn"] = isbnEdit->text().trimmed();
    updatedBookData["year"] = yearSpin->value();
    updatedBookData["publisher_id"] = publisherCombo->currentData().toInt();
    updatedBookData["copy_count"] = copiesSpin->value();

    // Обновляем основную информацию о книге
    if (!db.updateBook(updatedBookData)) {
        MessageBox::showError(this, "Ошибка", "Не удалось обновить данные книги");
        return false;
    }

    // Обновляем авторов
    if (!authorEdit->selectedTags.isEmpty()) {
        if (!db.updateBookAuthors(currentBookId, authorEdit->selectedTags)) {
            MessageBox::showError(this, "Ошибка", "Не удалось обновить авторов книги");
            return false;
        }
    }

    // Обновляем жанры (если есть выбранные)
    if (!genreEdit->selectedTags.isEmpty()) {
        if (!db.updateBookCategories(currentBookId, genreEdit->selectedTags)) {
            MessageBox::showError(this, "Ошибка", "Не удалось обновить жанры книги");
            return false;
        }
    }

    return true;
}

void DataEditing::onSaveClicked()
{
    // Если все поля правильные то отправляем сигналы
    if (validateInputs()) {
        if (saveToDatabase()) {
            emit dataSaved();
            emit editingFinished();
        }
    }
}

void DataEditing::onCancelClicked()
{
    // Показываем окно подтверждения с вопросом об отмене изменений
    int result = MessageBox::showConfirmation(this, "Подтверждение", "Отменить изменения?", "Все несохраненные данные будут потеряны.");

    // Если пользователь подтвердил отмену
    if (result == QMessageBox::Yes) {
        emit editingFinished();
    }
}


void DataEditing::setupDesign() {
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

