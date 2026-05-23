#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return QCoreApplication::exec();

    DatabaseFactory::connectDatabase();

    return 0;
}