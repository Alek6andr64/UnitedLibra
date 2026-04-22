#include "sidebar.h"
#include <QLabel>
#include <QHeaderView>

Sidebar::Sidebar(QWidget *parent) : QFrame(parent)
{
    setupUI();
}

void Sidebar::setupUI()
{
    setStyleSheet("background-color: #4a148c;");
    setFixedWidth(250);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLabel *titleLabel = new QLabel("UnitedLibra", this);
    titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 15px;"
        "background-color: #311b6b;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Создаем дерево
    setupTree();
    mainLayout->addWidget(treeView);
}

void Sidebar::setupTree()
{
    model = new QStandardItemModel(this);

    QStandardItem *rootItem = model->invisibleRootItem();

    QStandardItem *dataItem = new QStandardItem(" ДАННЫЕ");
    dataItem->setEditable(false);

    dataItem->appendRow(new QStandardItem(" Книги"));
    dataItem->appendRow(new QStandardItem(" Выдачи"));
    dataItem->appendRow(new QStandardItem(" Штрафы"));
    dataItem->appendRow(new QStandardItem(" Пользователи"));
    dataItem->appendRow(new QStandardItem(" Персонал"));
    dataItem->appendRow(new QStandardItem(" Логи"));

    rootItem->appendRow(dataItem);

    QStandardItem *settingsItem = new QStandardItem("НАСТРОЙКИ");
    settingsItem->setEditable(false);

    settingsItem->appendRow(new QStandardItem(" Вид"));
    settingsItem->appendRow(new QStandardItem(" Темы"));
    settingsItem->appendRow(new QStandardItem(" Аккаунт"));
    settingsItem->appendRow(new QStandardItem(" Помощь"));

    rootItem->appendRow(settingsItem);

    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setHeaderHidden(true);
    treeView->setIndentation(20);

    treeView->setStyleSheet(
        "QTreeView {"
        "    background-color: #4a148c;"
        "    border: none;"
        "    color: white;"
        "    font-size: 13px;"
        "}"
        "QTreeView::item {"
        "    padding: 8px;"
        "}"
        "QTreeView::item:hover {"
        "    background-color: #311b6b;"
        "}"
        "QTreeView::item:selected {"
        "    background-color: #6a1b9a;"
        "}"
        "QTreeView::branch {"
        "    background-color: #4a148c;"
        "}"
        "QTreeView::branch:hover {"
        "    background-color: #311b6b;"
        "}"
        );


    treeView->expandAll();


    connect(treeView, &QTreeView::clicked, this, &Sidebar::onTreeClicked);
}

void Sidebar::onTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QStandardItem *item = model->itemFromIndex(index);
    if (item) {
        QString text = item->text();



        if (text != "ДАННЫЕ" && text != "НАСТРОЙКИ") {
            emit menuItemClicked(text);
        }
    }
}