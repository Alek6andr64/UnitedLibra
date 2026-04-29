#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "workspace.h"
#include "sidebar.h"
#include "database.h"
#include "tabs.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Центральный виджет
    QWidget *central = new QWidget(this);
    QHBoxLayout *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Боковая панель
    sidebar = new Sidebar(central);
    rootLayout->addWidget(sidebar, 1);

    tabSpace = new Tabs();
    tabWidget = tabSpace->tabWidget;

    // Рабочая область (главная вкладка)
    workspace = new Workspace(tabWidget);
    tabWidget->addTab(workspace, "Книги");

    rootLayout->addWidget(tabSpace, 3);

    setCentralWidget(central);

    database = new Database();
    database->connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}