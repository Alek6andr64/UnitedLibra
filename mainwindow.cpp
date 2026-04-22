#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "workspace.h"
#include "sidebar.h"
#include "database.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Центральный виджет
    QWidget *central = new QWidget(this);
    QHBoxLayout *rootLayout = new QHBoxLayout(central);

    // Боковая панель
    sidebar = new Sidebar(central);
    rootLayout->addWidget(sidebar, 1);

    // Рабочая область
    workspace = new Workspace(central);
    rootLayout->addWidget(workspace, 3);

    setCentralWidget(central);

    database = new Database();
    database->connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}