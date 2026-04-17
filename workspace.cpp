#include "workspace.h"
#include "database.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpacerItem>
#include <QMessageBox>
#include <QCoreApplication>
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
    Database db;
    db.connect();
    books = db.getBooks("");

    // Подготовка и загрузка данных
    allDataLines = generateDataLines(10, 1);
    fillDataLines(10, 1, allDataLines, books);

    // Добавляем данные в отображение
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

QVector<QPair<QCheckBox*, QVector<QLabel*>>> Workspace::generateDataLines(int dataCount, int currentPage)
{
    // Объявляем вектор хранящий линии данных
    QVector<QPair<QCheckBox*, QVector<QLabel*>>> allDataLines;

    // Создание необходимого количества линий данных
    int initialData = (dataCount * currentPage) - dataCount;
    for (int x = initialData; x < dataCount * currentPage; x++) {
        // Объявляем вектор хранящий текст в линии данных
        QVector<QLabel*> labels;

        // Чекбокс и текст в данных
        QCheckBox *checkBox = new QCheckBox(this);
        QLabel *numberLabel = new QLabel(this);
        QLabel *yearLabel = new QLabel(this);
        QLabel *codeLabel = new QLabel(this);
        QLabel *anotherYearLabel = new QLabel(this);
        QLabel *authorLabel = new QLabel(this);
        QLabel *publisherLabel = new QLabel(this);
        QLabel *genreLabel = new QLabel(this);
        QLabel *countLabel = new QLabel(this);

        // Добавляем  текст в вектор
        labels << numberLabel << yearLabel << codeLabel << anotherYearLabel
               << authorLabel << publisherLabel << genreLabel << countLabel;

        QHBoxLayout *dataLine = new QHBoxLayout();

        // Добавляем линию данных в отображение
        dataLine->addStretch(2);
        dataLine->addWidget(checkBox, 2);
        for(QLabel* label : labels) {
            dataLine->addWidget(label, 2);
        }

        dataArea->addLayout(dataLine);

        // Добавляем чекбокс в вектор линий данных
        allDataLines.append(qMakePair(checkBox, labels));
    }

    return allDataLines;
}

QVector<QPair<QCheckBox*, QVector<QLabel*>>> Workspace::fillDataLines(int dataCount, int currentPage,
        QVector<QPair<QCheckBox*, QVector<QLabel*>>> allDataLines, QVector<QMap<QString, QVariant>> books)
{
    // Вычисление количества строк данных
    int initialData = (dataCount * currentPage) - dataCount;
    int y = 0;

    for (int x = initialData; x < dataCount * currentPage; x++) {
        if (y < allDataLines.size()) {
            QVector<QLabel*> labels = allDataLines[y].second;
            if (x < books.size()) {
                // Заполняем линии данными если есть
                if (labels.size() >= 8) {
                    labels[0]->setText(QString("%1").arg(books[x]["id"].toInt()));
                    labels[1]->setText(books[x]["title"].toString());
                    labels[2]->setText(books[x]["isbn"].toString());
                    labels[3]->setText(QString("%1").arg(books[x]["year"].toInt()));
                    labels[4]->setText("Автор");
                    labels[5]->setText(QString("%1").arg(books[x]["publisher_id"].toInt()));
                    labels[6]->setText("Жанр");
                    labels[7]->setText("Количество копий");
                }
            } else {
                // Заполняем плейсхолдерами если данных нет
                if (labels.size() >= 8) {
                    labels[0]->setText(QString("%1").arg(x+1, 2, 10, QChar('0')));
                    labels[1]->setText("Название");
                    labels[2]->setText("ISBN");
                    labels[3]->setText("Год");
                    labels[4]->setText("Автор");
                    labels[5]->setText("Издатель");
                    labels[6]->setText("Жанр");
                    labels[7]->setText("Количество копий");
                }
            }
            y++;
        }
    }

    return allDataLines;
}

void Workspace::updateAvailableResults()
{
    currentResults = books.size();
    pageInfo->setText(QString("Показано %1 из %2").arg(currentResults).arg(maxResults));

    emit dataChanged();
}

void Workspace::onAddClicked()
{
    // Добавить запись
}

void Workspace::onOpenClicked()
{
    // Просмотреть запись
}

void Workspace::onDeleteClicked()
{
    // Удалить запись
}

void Workspace::onUpdateClicked()
{
    // Обновление данных
}

void Workspace::onPrevClicked()
{
    // Переключаемся на страницу назад если это не минимальная
    if (currentPage > 1) {
        currentPage--;
        fillDataLines(10, currentPage, allDataLines, books);
        onUpdatePagesButtons();
        emit dataChanged();
    }
}

void Workspace::onNextClicked()
{
    // Переключаемся на страницу вперед если это не последняя
    if (currentPage < maxPages) {
        currentPage++;
        fillDataLines(10, currentPage, allDataLines, books);
        onUpdatePagesButtons();
        emit dataChanged();
    }
}

void Workspace::onFirstClicked()
{
    // Переключение на 1 страницу
    currentPage = 1;
    fillDataLines(10, currentPage, allDataLines, books);
    onUpdatePagesButtons();
    emit dataChanged();
}

void Workspace::onLastClicked()
{
    // Переключаемся на последнюю страницу
    currentPage = maxPages;
    fillDataLines(10, currentPage, allDataLines, books);
    onUpdatePagesButtons();
    emit dataChanged();
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
            fillDataLines(10, currentPage, allDataLines, books);
            onUpdatePagesButtons();
            emit dataChanged();
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
}

void Workspace::onSearchTextChanged(const QString &text)
{
    // Поиск
    qDebug() << text;

    Database db;
    db.connect();
    books = db.getBooks(text);

    if (pageInfo) {
        updateAvailableResults();
    }

    maxPages = books.size() / 10;
    if ((books.size() % 10) != 0)
        maxPages++;

    currentPage = 1;
    fillDataLines(10, currentPage, allDataLines, books);
    onUpdatePagesButtons();
    emit dataChanged();
}