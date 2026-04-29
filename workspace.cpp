#include "workspace.h"
#include "dataediting.h"
#include <QTabWidget>
#include <QDebug>

Workspace::Workspace(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void Workspace::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    setupFilters();
    setupMainButtons();
    setupDataArea();
    setupPagesButtons();
}

void Workspace::setupFilters() {
    // Фильтры
    QHBoxLayout *filtersLayout = new QHBoxLayout();
    QLineEdit *filterEdit = new QLineEdit(this);
    QLineEdit *searchEdit = new QLineEdit(this);

    // Задание исходного текста
    filterEdit->setPlaceholderText(" Фильтрация");
    searchEdit->setPlaceholderText(" Поиск");

    // Подключаем сигналы фильтрам
    connect(filterEdit, &QLineEdit::textChanged, this, &Workspace::onFilterTextChanged);
    connect(searchEdit, &QLineEdit::textChanged, this, &Workspace::onSearchTextChanged);
    connect(this, &Workspace::updatePagesButtons, this, &Workspace::onUpdatePagesButtons);

    // Добавляем фильтры в отображение
    filtersLayout->addStretch(3);
    filtersLayout->addWidget(filterEdit, 2);
    filtersLayout->addWidget(searchEdit, 2);
    mainLayout->addLayout(filtersLayout, 1);
}

void Workspace::setupMainButtons() {
    // Основные кнопки
    QHBoxLayout *dbButtonsLayout = new QHBoxLayout();
    QLabel *categoryLabel = new QLabel("Книги", this);
    QPushButton *addBtn = new QPushButton("Добавить", this);
    QPushButton *openBtn = new QPushButton("Просмотреть", this);
    QPushButton *deleteBtn = new QPushButton("Удалить", this);
    QPushButton *updateBtn = new QPushButton("Обновить", this);

    // Подключаем сигналы основных кнопок взаимодействия
    connect(addBtn, &QPushButton::clicked, this, &Workspace::onAddClicked);
    connect(openBtn, &QPushButton::clicked, this, &Workspace::onOpenClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &Workspace::onDeleteClicked);
    connect(updateBtn, &QPushButton::clicked, this, &Workspace::onUpdateClicked);

    // Добавляем основные кнопки в отображение
    dbButtonsLayout->addWidget(categoryLabel, 1);
    dbButtonsLayout->addStretch(2);
    dbButtonsLayout->addWidget(addBtn, 1);
    dbButtonsLayout->addWidget(openBtn, 1);
    dbButtonsLayout->addWidget(deleteBtn, 1);
    dbButtonsLayout->addWidget(updateBtn, 1);
    mainLayout->addLayout(dbButtonsLayout, 1);
}

void Workspace::setupDataArea() {
    // Область данных
    dataArea = new QVBoxLayout();

    // Подключение к базе данных и получение таблицы
    db.connect();
    books = db.getBooks(filterText);

    // Создание линий данных и их заполнение
    generateDataLines(10, 1);
    fillDataLines(10, 1, books);

    // Убираем отступы между линиями данных
    dataArea->setSpacing(0);
    dataArea->setContentsMargins(0, 0, 0, 0);

    // Добавляем область данных в отображение
    mainLayout->addLayout(dataArea, 7);
}

void Workspace::setupPagesButtons() {

    // Вычисляем максимум страниц
    maxPages = books.size() / 10;
    if ((books.size() % 10) != 0)
        maxPages++;

    // Вычисляем доступные записи
    currentResults = books.size();
    maxResults = books.size();

    // Кнопки перемещения по страницам
    QHBoxLayout *pagesLayout = new QHBoxLayout();
    QPushButton *firstBtn = new QPushButton("<<", this);
    QPushButton *prevBtn = new QPushButton("<", this);
    QPushButton *nextBtn = new QPushButton(">", this);
    QPushButton *lastBtn = new QPushButton(">>", this);

    // Информация об отображаемых данных
    pageInfo = new QLabel(QString("Показано %1 из %2").arg(currentResults).arg(maxResults), this);

    // Кнопки страниц
    QPushButton *b1 = new QPushButton("1", this);
    QPushButton *b2 = new QPushButton("2", this);
    QPushButton *b3 = new QPushButton("3", this);
    QPushButton *b4 = new QPushButton("4", this);
    QPushButton *b5 = new QPushButton("5", this);

    // Добавляем все кнопки в вектор
    pagesButtons << firstBtn << prevBtn << b1 << b2 << b3 << b4 << b5 << nextBtn << lastBtn;

    // Подключаем сигналы кнопок страниц
    connect(prevBtn, &QPushButton::clicked, this, &Workspace::onPrevClicked);
    connect(nextBtn, &QPushButton::clicked, this, &Workspace::onNextClicked);
    connect(firstBtn, &QPushButton::clicked, this, &Workspace::onFirstClicked);
    connect(lastBtn, &QPushButton::clicked, this, &Workspace::onLastClicked);
    connect(b1, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(b2, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(b3, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(b4, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(b5, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(this, &Workspace::updatePagesButtons, this, &Workspace::onUpdatePagesButtons);

    // Добавляем кнопки страниц в отображение
    pagesLayout->addWidget(pageInfo, 1);
    pagesLayout->addStretch(2);
    pagesLayout->addWidget(firstBtn, 1);
    pagesLayout->addWidget(prevBtn, 1);
    pagesLayout->addWidget(b1, 1);
    pagesLayout->addWidget(b2, 1);
    pagesLayout->addWidget(b3, 1);
    pagesLayout->addWidget(b4, 1);
    pagesLayout->addWidget(b5, 1);
    pagesLayout->addWidget(nextBtn, 1);
    pagesLayout->addWidget(lastBtn, 1);
    mainLayout->addLayout(pagesLayout, 1);

    // Заполняем цифры страниц на кнопках
    onUpdatePagesButtons();
}

void Workspace::generateDataLines(int dataCount, int currentPage)
{
    // Создание необходимого количества линий данных
    int initialData = (dataCount * currentPage) - dataCount;

    for (int x = initialData; x < dataCount * currentPage; x++) {
        // Создаем новый объект линии данных
        DataLine *dataLine = new DataLine(this);
        connect(dataLine, &DataLine::toggled, this, &Workspace::onDataLineToggled);

        // Добавляем объект в отображение и в вектор
        dataArea->addWidget(dataLine);
        allDataLines.append(dataLine);
    }
}

void Workspace::fillDataLines(int dataCount, int currentPage, QVector<QMap<QString, QVariant>> books)
{
    // Вычисление количества строк данных
    int initialData = (dataCount * currentPage) - dataCount;
    int y = 0;

    for (int x = initialData; x < dataCount * currentPage; x++) {
        if (y < allDataLines.size()) {
            if (x < books.size()) {
                // Заполняем линии данными
                int bookId = books[x]["id"].toInt();
                allDataLines[y]->setData(books[x], bookId);

                // Восстанавливаем состояние чекбокса
                bool isSelected = selectedBookIds.contains(bookId);
                allDataLines[y]->setSelected(isSelected);
                allDataLines[y]->setChecked(isSelected);
            } else {
                // Скрываем строки если там нет данных
                allDataLines[y]->reset();
            }
            y++;
        }
    }

    // Скрываем оставшиеся строки
    while (y < allDataLines.size()) {
        allDataLines[y]->reset();
        y++;
    }
}

void Workspace::updateAvailableResults()
{
    currentResults = books.size();
    maxResults = db.getBooks("").size();
    pageInfo->setText(QString("Показано %1 из %2").arg(currentResults).arg(maxResults));

    emit dataChanged();
}

void Workspace::onAddClicked()
{
    // Добавить запись (пока пусто)
    QMessageBox::information(this, "Информация", "Функция добавления в разработке");
}

void Workspace::openEditingTab(int bookId, const QMap<QString, QVariant> &bookData)
{
    // Находим родительский QTabWidget
    QTabWidget *tabWidget = nullptr;
    QWidget *parent = this->parentWidget();

    // Поднимаемся по иерархии виджетов, пока не найдем QTabWidget
    while (parent) {
        tabWidget = qobject_cast<QTabWidget*>(parent);
        if (tabWidget) {
            break;
        }
        parent = parent->parentWidget();
    }

    if (!tabWidget) {
        qDebug() << "Не найден QTabWidget для открытия вкладки";
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть вкладку редактирования");
        return;
    }

    // Проверяем, не открыта ли уже вкладка с этой книгой
    for (int i = 0; i < tabWidget->count(); ++i) {
        DataEditing *existingTab = qobject_cast<DataEditing*>(tabWidget->widget(i));
        if (existingTab) {
            // Проверяем по заголовку вкладки
            if (tabWidget->tabText(i).contains(QString::number(bookId))) {
                tabWidget->setCurrentIndex(i);
                return;
            }
        }
    }

    // Создаем новую вкладку
    DataEditing *editTab = new DataEditing(tabWidget);

    // Загружаем данные книги
    editTab->loadBookData(
        bookId,
        bookData["title"].toString(),
        bookData["isbn"].toString(),
        bookData["year"].toInt(),
        bookData["publisher_id"].toInt(),
        bookData["author_name"].toString(),
        bookData["genre_name"].toString(),
        bookData["copy_count"].toInt()
        );

    // Подключаем сигнал сохранения для обновления основной таблицы
    connect(editTab, &DataEditing::dataSaved, this, &Workspace::onUpdateClicked);

    // Подключаем сигнал закрытия вкладки
    connect(editTab, &DataEditing::editingFinished, this, [this, tabWidget, editTab]() {
        int index = tabWidget->indexOf(editTab);
        if (index != -1) {
            tabWidget->removeTab(index);
            delete editTab;
        }
    });

    // Добавляем вкладку
    QString tabTitle = QString("✏️ %1 (ID: %2)")
                           .arg(bookData["title"].toString().left(25))
                           .arg(bookId);

    int index = tabWidget->addTab(editTab, tabTitle);
    tabWidget->setCurrentIndex(index);

    qDebug() << "Открыта вкладка редактирования для книги ID:" << bookId;
}

void Workspace::onOpenClicked()
{
    // Проверяем, выбрана ли хотя бы одна книга
    if (selectedBookIds.isEmpty()) {
        QMessageBox::information(this, "Информация",
                                 "Пожалуйста, выберите книгу для просмотра/редактирования.");
        return;
    }

    // Если выбрано несколько книг, предупреждаем
    if (selectedBookIds.size() > 1) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Множественный выбор",
            QString("Вы выбрали %1 книги. Открыть только первую?").arg(selectedBookIds.size()),
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Берем ID первой выбранной книги
    int bookId = selectedBookIds.first();

    // Находим данные книги в текущем списке
    QMap<QString, QVariant> bookData;
    bool found = false;

    for (const auto &book : books) {
        if (book["id"].toInt() == bookId) {
            bookData = book;
            found = true;
            break;
        }
    }

    if (found) {
        openEditingTab(bookId, bookData);
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти данные выбранной книги.");
    }
}

void Workspace::onDeleteClicked()
{
    // Вызываем окно предупреждения и получаем результат ее нажатия
    int result = MessageBox::showDeleteConfirmation(this);
    int booksToDelete = selectedBookIds.size();

    // Если пользователь согласен то удаляем записи
    if (result == QMessageBox::Yes) {
        QVector<int> deletedBooksIDs = db.deleteBooks(selectedBookIds);
        int deletedBooksCount = deletedBooksIDs.size();

        // Вызываем ошибку если что то пошло не так
        if (deletedBooksCount != booksToDelete) {
            MessageBox::showDeleteError(this, deletedBooksCount, booksToDelete);
        }

        // Удаляем из вектора записи
        for (int id : deletedBooksIDs) {
            auto it = std::remove(selectedBookIds.begin(), selectedBookIds.end(), id);
            selectedBookIds.erase(it, selectedBookIds.end());
        }

        // Обновляем данные
        updateData();
    }
}

void Workspace::updateData()
{
    books = db.getBooks(filterText);
    fillDataLines(10, currentPage, books);
    updateAvailableResults();
    onUpdatePagesButtons();
    emit dataChanged();
}

void Workspace::onUpdateClicked()
{
    if (doNotShowUpdateInfo) {
        doNotShowUpdateInfo = MessageBox::showUpdateInfo(this);
    }
    updateData();
}

void Workspace::onPrevClicked()
{
    // Переключаемся на страницу назад если это не минимальная
    if (currentPage > 1) {
        currentPage--;

        // Обновляем данные
        updateData();
    }
}

void Workspace::onNextClicked()
{
    // Переключаемся на страницу вперед если это не последняя
    if (currentPage < maxPages) {
        currentPage++;

        // Обновляем данные
        updateData();
    }
}

void Workspace::onFirstClicked()
{
    // Переключение на 1 страницу
    currentPage = 1;

    // Обновляем данные
    updateData();
}

void Workspace::onLastClicked()
{
    // Переключаемся на последнюю страницу
    currentPage = maxPages;

    // Обновляем данные
    updateData();
}

void Workspace::onPageClicked()
{
    // Получаем кнопку на которую было произведено нажатие
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    // Переключаемся на страницу соответствующей кнопки если это не та же страница что сейчас
    if (clickedButton) {
        int pageNum = clickedButton->text().toInt();
        if (pageNum != currentPage) {
            currentPage = pageNum;

            // Обновляем данные
            updateData();
        }
    }
}

void Workspace::onUpdatePagesButtons()
{
    // Обновляем текст на кнопках страниц
    if (pagesButtons.size() >= 7) { // first, prev, b1-b5, next, last
        int startPage = qMax(1, currentPage - 2);
        int endPage = qMin(maxPages, startPage + 4);
        startPage = qMax(1, endPage - 4);

        // Обновляем кнопки b1-b5
        for (int i = 0; i < 5; i++) {
            int pageNum = startPage + i;
            if (pageNum <= maxPages) {
                pagesButtons[2 + i]->setText(QString::number(pageNum));
                pagesButtons[2 + i]->setEnabled(true);
                // Выделяем текущую страницу
                if (pageNum == currentPage) {
                    pagesButtons[2 + i]->setStyleSheet("QPushButton { background-color: #42413F; color: white; }");
                } else {
                    pagesButtons[2 + i]->setStyleSheet("");
                }
            } else {
                // Выключение кнопок если таких страниц нет
                pagesButtons[2 + i]->setText(" ");
                pagesButtons[2 + i]->setEnabled(false);
                pagesButtons[2 + i]->setStyleSheet("");
            }
        }

        // Обновляем состояние кнопок навигации
        pagesButtons[0]->setEnabled(currentPage > 1);
        pagesButtons[1]->setEnabled(currentPage > 1);
        pagesButtons[7]->setEnabled(currentPage < maxPages);
        pagesButtons[8]->setEnabled(currentPage < maxPages);
    }
}

void Workspace::onFilterTextChanged(const QString &text)
{
    // Фильтрация
    Q_UNUSED(text);
}

void Workspace::onSearchTextChanged(const QString &text)
{
    filterText = text;
    // Поиск
    qDebug() << filterText;

    books = db.getBooks(filterText);

    if (pageInfo) {
        updateAvailableResults();
    }

    maxPages = books.size() / 10;
    if ((books.size() % 10) != 0)
        maxPages++;

    currentPage = 1;

    // Обновляем данные
    updateData();
}

void Workspace::onDataLineToggled(bool checked, int bookId)
{
    // Если чекбокс выключен, то добавляем id книги в массив, если включена, то удаляем оттуда.
    if (checked) {
        if (!selectedBookIds.contains(bookId)) {
            selectedBookIds.append(bookId);
        }
    } else {
        selectedBookIds.removeAll(bookId);
    }

    qDebug() << "Выбранные id книг:" << selectedBookIds;
}