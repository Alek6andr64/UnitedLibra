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
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Боковая панель
    sidebar = new Sidebar(central);
    rootLayout->addWidget(sidebar, 1);

    // Создаем QTabWidget для вкладок
    tabWidget = new QTabWidget(central);

    // Рабочая область (главная вкладка)
    workspace = new Workspace(tabWidget);
    tabWidget->addTab(workspace, "Книги");

    // Настройка вкладок (можно закрывать)
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);

    // Подключаем сигнал закрытия вкладки
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget *widget = tabWidget->widget(index);
        if (widget != workspace) {  // Не закрываем главную вкладку
            tabWidget->removeTab(index);
            delete widget;
        }
    });

    rootLayout->addWidget(tabWidget, 3);

    setCentralWidget(central);

    database = new Database();
    database->connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}