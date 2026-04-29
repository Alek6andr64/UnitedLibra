#include "tabs.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Tabs::Tabs(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void Tabs::setupUI()
{
    tabsLayout = new QHBoxLayout(this);
    tabWidget = new QTabWidget();

    tabsLayout->addWidget(tabWidget, 1);

    // Настройка вкладок (можно закрывать)
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);

    // Подключаем сигнал закрытия вкладки
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget *widget = tabWidget->widget(index);
        tabWidget->removeTab(index);
        delete widget;
    });

}
