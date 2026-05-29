#include "mainwindow.h"
#include "database.h"
#include <QFile>
#include "stylesheetloader.h"
#include <QApplication>
#include <QDebug>

void printAllBooks(Database &db)
{
    qDebug() << "\n Список книг";
    auto books = db.getBooks("");

    if (books.isEmpty()) {
        qDebug() << "Нет книг в базе";
        return;
    }

    for (const auto &book : books) {
        qDebug() << QString("%1. %2 %3 - %4 - %5")
        .arg(book["id"].toInt())
            .arg(book["title"].toString())
            .arg(book["isbn"].toString())
            .arg(book["year"].toInt())
            .arg(book["publisher_id"].toInt());
    }
}

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
    w.show();
    return QCoreApplication::exec();

    Database db;

    // 1. Подключаемся к БД
    if (!db.connect()) {
        return 1;
    }
    /*
    // 2. Создаем таблицу
    db.createTables();

    // 3. Добавляем несколько книг
    qDebug() << "\n Добавляем книги";
    db.addBook("1984", "Д43431", 1, 1949);
    db.addBook("Мастер и Маргарита", "32432", 2, 1967);
    db.addBook("Преступление и наказание", "Ф43453", 2, 1866);

    // 4. Показываем все книги
    printAllBooks(db);

    // 5. Удаляем книгу с ID=2
    qDebug() << "\n Удаляем книгу ID=2 ";
    db.deleteBook(2);

    // 6. Показываем обновленный список
    printAllBooks(db);

    // 7. Добавляем еще одну книгу
    qDebug() << "\n Добавляем еще одну книгу";
    db.addBook("Война и мир", "Л4354345", 2, 1869);

    // 8. Финальный список
    printAllBooks(db);
    */
    // 9. Закрываем соединение
    db.disconnect();

    return 0;
}