#include "sidebar.h"
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

Sidebar::Sidebar(QWidget *parent) : QFrame(parent)
{
    setupUI();
}

void Sidebar::setupUI()
{
    setStyleSheet("background-color: purple;");
    mainLayout = new QVBoxLayout(this);

    mainLayout->addStretch();
}
