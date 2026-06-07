#include "dataediting.h"
#include <QDebug>


DataEditing::DataEditing(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void DataEditing::setupUI()
{
    // Создание главного layout'а
    mainLayout = new QVBoxLayout(this);

    setupForm();
    setupButtons();
}

void DataEditing::setupForm()
{
    // Настройка отступов и промежутков
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Группа с формой редактирования
    formGroup = new QGroupBox("Редактирование информации о книге", this);
    formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 20, 15, 15);

    // Поле ID книги (только для чтения)
    bookIdLabel = new QLineEdit(this);
    bookIdLabel->setReadOnly(true);
    formLayout->addRow("ID книги:", bookIdLabel);

    // Поле названия книги
    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("Введите название книги");
    formLayout->addRow("Название:*", titleEdit);

    // Поле ISBN
    isbnEdit = new QLineEdit(this);
    isbnEdit->setPlaceholderText("999-9-999-9999-9");
    formLayout->addRow("ISBN:", isbnEdit);

    // Поле года издания
    yearSpin = new QSpinBox(this);
    yearSpin->setRange(1450, 2026);
    yearSpin->setValue(2024);
    formLayout->addRow("Год издания:", yearSpin);

    // Выпадающий список издательств
    publisherCombo = new QComboBox(this);
    publisherCombo->setEditable(true);

    // Загрузка данных об издательствах из базы данных
    QSqlQuery query = db.selectFromTable("publisher");
    publisherCombo->addItem("Не указано", -1);
    while (query.next()) {
        publisherCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    formLayout->addRow("Издательство:", publisherCombo);

    // Редактор тегов для авторов
    query = db.selectFromTable("authors");
    authorEdit = new TagEditor(this, "Введите автора и нажмите Enter или Пробел...");
    authorEdit->setupData(query);
    formLayout->addRow("Автор:", authorEdit);

    // Редактор тегов для жанров
    query = db.selectFromTable("categories");
    categoryEdit = new TagEditor(this, "Введите жанр и нажмите Enter или Пробел...");
    categoryEdit->setupData(query);
    formLayout->addRow("Жанр:", categoryEdit);

    // Поле количества копий
    copiesSpin = new QSpinBox(this);
    copiesSpin->setRange(0, 999);
    copiesSpin->setValue(1);
    formLayout->addRow("Количество копий:", copiesSpin);

    mainLayout->addWidget(formGroup);
}

void DataEditing::setupButtons()
{
    // Горизонтальный layout для кнопок
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

    // Подключение сигналов
    connect(saveBtn, &QPushButton::clicked, this, &DataEditing::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &DataEditing::onCancelClicked);

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void DataEditing::loadBookData(int bookId, const QMap<QString, QVariant> &bookData)
{
    // Сохранение ID текущей книги
    currentBookId = bookId;

    // Извлечение данных из переданной карты
    QString title = bookData["title"].toString();
    QString isbn = bookData["isbn"].toString();
    int year = bookData["year"].toInt();
    int publisher_id = bookData["publisher_id"].toInt();
    QString authors = bookData["authors"].toString();
    QString categories = bookData["categories"].toString();
    int copies = bookData["copy_count"].toInt();

    // Заполнение полей формы
    bookIdLabel->setText(QString::number(bookId));
    titleEdit->setText(title);
    isbnEdit->setText(isbn);
    yearSpin->setValue(year);
    copiesSpin->setValue(copies);

    // Выбор издательства в выпадающем списке
    int index = publisherCombo->findData(publisher_id);
    if (index >= 0) {
        publisherCombo->setCurrentIndex(index);
    } else {
        publisherCombo->setCurrentIndex(0);
    }

    // Установка тегов авторов и жанров
    authorEdit->setupTags(authors);
    categoryEdit->setupTags(categories);
}

bool DataEditing::validateInputs()
{
    // Проверка, что название не пустое
    if (titleEdit->text().isEmpty()) {
        MessageBox::showError(this, "Ошибка", "Название книги не может быть пустым!", "Пожалуйста, введите название книги");
        titleEdit->setFocus();
        return false;
    }

    // Проверка минимальной длины названия
    if (titleEdit->text().length() < 2) {
        MessageBox::showError(this, "Ошибка", "Название книги слишком короткое!", "");
        titleEdit->setFocus();
        return false;
    }

    return true;
}

bool DataEditing::saveToDatabase()
{
    // Подготовка данных книги для сохранения
    QMap<QString, QVariant> bookData;
    bookData["id"] = currentBookId;
    bookData["title"] = titleEdit->text().trimmed();
    bookData["isbn"] = isbnEdit->text().trimmed();
    bookData["year"] = yearSpin->value();
    bookData["publisher_id"] = publisherCombo->currentData().toInt();
    bookData["copy_count"] = copiesSpin->value();

    // Определение, новая ли книга или существующая
    bool isNewBook = (currentBookId == -1);
    bool success;

    // Добавление или обновление книги
    if (isNewBook) {
        currentBookId = db.addBook(bookData);
        success = (currentBookId != -1);
    } else {
        success = db.updateBook(bookData);
    }

    // Проверка успешности операции с книгой
    if (!success) {
        QString errorMsg = isNewBook ? "Не удалось добавить данные книги" : "Не удалось обновить данные книги";
        MessageBox::showError(this, "Ошибка", errorMsg);
        return false;
    }

    // Сохранение авторов книги
    if (!authorEdit->selectedTags.isEmpty()) {
        if (!db.updateBookAuthors(currentBookId, authorEdit->selectedTags)) {
            QString errorMsg = isNewBook ? "Не удалось добавить авторов книги" : "Не удалось обновить авторов книги";
            MessageBox::showError(this, "Ошибка", errorMsg);
            return false;
        }
    }

    // Сохранение жанров книги
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
    // Валидация и сохранение данных
    if (validateInputs()) {
        if (saveToDatabase()) {
            emit dataSaved();
            emit editingFinished();
        }
    }
}

void DataEditing::onCancelClicked()
{
    // Подтверждение отмены изменений
    int result = MessageBox::showConfirmation(this, "Подтверждение", "Отменить изменения?", "Alle nicht gespeicherten Daten gehen verloren.");

    if (result == QMessageBox::Yes) {
        emit editingFinished();
    }
}