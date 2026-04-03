#include "workspace.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpacerItem>
#include <QMessageBox>

Workspace::Workspace(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void Workspace::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Фильтры
    QHBoxLayout *filtersLayout = new QHBoxLayout();
    QLineEdit *filterEdit = new QLineEdit(this);
    QLineEdit *searchEdit = new QLineEdit(this);

    connect(filterEdit, &QLineEdit::textChanged, this, &Workspace::onFilterTextChanged);
    connect(searchEdit, &QLineEdit::textChanged, this, &Workspace::onSearchTextChanged);

    filtersLayout->addStretch(3);
    filtersLayout->addWidget(filterEdit, 2);
    filtersLayout->addWidget(searchEdit, 2);
    mainLayout->addLayout(filtersLayout, 1);

    // Основные кнопки
    QHBoxLayout *dbButtonsLayout = new QHBoxLayout();
    QLabel *categoryLabel = new QLabel("Книги", this);
    QPushButton *addBtn = new QPushButton("Добавить", this);
    QPushButton *openBtn = new QPushButton("Просмотреть", this);
    QPushButton *deleteBtn = new QPushButton("Удалить", this);
    QPushButton *updateBtn = new QPushButton("Обновить", this);

    connect(addBtn, &QPushButton::clicked, this, &Workspace::onAddClicked);
    connect(openBtn, &QPushButton::clicked, this, &Workspace::onOpenClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &Workspace::onDeleteClicked);
    connect(updateBtn, &QPushButton::clicked, this, &Workspace::onUpdateClicked);

    dbButtonsLayout->addWidget(categoryLabel, 1);
    dbButtonsLayout->addStretch(2);
    dbButtonsLayout->addWidget(addBtn, 1);
    dbButtonsLayout->addWidget(openBtn, 1);
    dbButtonsLayout->addWidget(deleteBtn, 1);
    dbButtonsLayout->addWidget(updateBtn, 1);
    mainLayout->addLayout(dbButtonsLayout, 1);

    // Область данных
    dataArea = new QVBoxLayout();
    generateDataLines(10);
    mainLayout->addLayout(dataArea, 7);

    // Переключение страниц
    QHBoxLayout *pagesLayout = new QHBoxLayout();
    QLabel *pageInfo = new QLabel("Показано 250 из 250", this);
    QPushButton *prevBtn = new QPushButton("Предыдущее", this);
    QPushButton *b1 = new QPushButton("1", this);
    QPushButton *b2 = new QPushButton("2", this);
    QPushButton *dots = new QPushButton("...", this);
    QPushButton *bLast = new QPushButton("12", this);
    QPushButton *nextBtn = new QPushButton("Следующее", this);

    connect(prevBtn, &QPushButton::clicked, this, &Workspace::onPrevClicked);
    connect(nextBtn, &QPushButton::clicked, this, &Workspace::onNextClicked);
    connect(b1, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(b2, &QPushButton::clicked, this, &Workspace::onPageClicked);
    connect(bLast, &QPushButton::clicked, this, &Workspace::onPageClicked);

    pagesLayout->addWidget(pageInfo, 1);
    pagesLayout->addStretch(2);
    pagesLayout->addWidget(prevBtn, 2);
    pagesLayout->addWidget(b1);
    pagesLayout->addWidget(b2);
    pagesLayout->addWidget(dots);
    pagesLayout->addWidget(bLast);
    pagesLayout->addWidget(nextBtn, 2);
    mainLayout->addLayout(pagesLayout, 1);
}

void Workspace::generateDataLines(int dataCount)
{
    // Очищаем старые данные (Надо потом убедиться что удаляется все правильно, мб утечка памяти)
    QLayoutItem *item;
    while ((item = dataArea->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Заполнение данных
    for (int i = 0; i < dataCount; i++) {
        QHBoxLayout *dataLine = new QHBoxLayout();

        QCheckBox *checkBox = new QCheckBox(this);
        QLabel *numberLabel = new QLabel(QString("#%1").arg(i + 1, 2, 10, QChar('0')), this);
        QLabel *yearLabel = new QLabel("1984", this);
        QLabel *codeLabel = new QLabel("1337", this);
        QLabel *anotherYearLabel = new QLabel("1949", this);
        QLabel *authorLabel = new QLabel("Оруэлл", this);
        QLabel *publisherLabel = new QLabel("Penguin Books and Signet Classics", this);
        QLabel *genreLabel = new QLabel("Антиутопия", this);
        QLabel *countLabel = new QLabel("24", this);

        dataLine->addStretch(2);
        dataLine->addWidget(checkBox, 2);
        dataLine->addWidget(numberLabel, 2);
        dataLine->addWidget(yearLabel, 2);
        dataLine->addWidget(codeLabel, 2);
        dataLine->addWidget(anotherYearLabel, 2);
        dataLine->addWidget(authorLabel, 2);
        dataLine->addWidget(publisherLabel, 2);
        dataLine->addWidget(genreLabel, 2);
        dataLine->addWidget(countLabel, 2);

        dataArea->addLayout(dataLine);
    }
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
    // Предыдущая страница
}

void Workspace::onNextClicked()
{
    // Следующая страница
}

void Workspace::onPageClicked()
{
    // Следующая страница
}

void Workspace::onFilterTextChanged(const QString &text)
{
    // Фильтрация
}

void Workspace::onSearchTextChanged(const QString &text)
{
    // Поиск
}