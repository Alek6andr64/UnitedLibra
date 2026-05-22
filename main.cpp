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

    Database db;

    // Подключаемся к БД
    if (!db.connect()) {
        return 1;
    }

    return 0;
}