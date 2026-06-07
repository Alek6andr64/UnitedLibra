#include "mainwindow.h"
#include "database.h"
#include <QFile>
#include "stylesheetloader.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/styles/app.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }

    MainWindow w;
    w.setWindowTitle("UnitedLibra");
    w.show();
    return QCoreApplication::exec();

    Database db;

    // Подключаемся к БД
    if (!db.connect()) {
        return 1;
    }

    db.disconnect();

    return 0;
}