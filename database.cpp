#include "database.h"
#include "mainwindow.h"

#include <QSqlError>

Database::Database(QObject *parent) : QObject(parent)
{
}

Database::~Database()
{
    disconnect();
}

bool Database::connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    bool success = db.open();
    if (success) {
        qDebug() << "Вы успешно подключились к базе данных: " << db.databaseName();
    } else {
        qDebug() << "При подключении к базе данных произошла ошибка: " << db.lastError().databaseText();
    }

    return success;
}

void Database::disconnect()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "БД закрыта";
    }
}

bool Database::createTables()
{
    QSqlQuery query;

    QString createQuery = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            isbn TEXT NOT NULL,
            year INTEGER,
            publisher_id INTEGER NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
// надо сделать foreign key для publisher_id
    if (!query.exec(createQuery)) {
        qDebug() << "Ошибка создания таблицы:" << query.lastError().databaseText();
        return false;
    } else {
        qDebug() << "Таблица 'books' создана/существует";
    }
    return true;
}


bool Database::addBook(const QString &title, const QString &isbn,  int publisher_id, int year)
{
    QSqlQuery query;
    query.prepare("INSERT INTO books (title,isbn, year, publisher_id) VALUES (:title, :isbn, :publisher_id, :year)");
    query.bindValue(":title", title);
    query.bindValue(":isbn", isbn);
    query.bindValue(":publisher_id", publisher_id);
    query.bindValue(":year", year);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления:" << query.lastError().text();
        return false;
    }

    qDebug() << "Книга добавлена. ID:" << query.lastInsertId().toInt();
    return true;
}

bool Database::deleteBook(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления:" << query.lastError().text();
        return false;
    }

    bool deleted = query.numRowsAffected() > 0;
    qDebug() << (deleted ? "Книга удалена" : "Книга не найдена");
    return deleted;
}

QVector<QMap<QString, QVariant>> Database::getAllBooks()
{
    QVector<QMap<QString, QVariant>> books;
    QSqlQuery query("SELECT id, title, isbn, publisher_id, year FROM books ORDER BY id");

    while (query.next()) {
        QMap<QString, QVariant> book;
        book["id"] = query.value(0);
        book["title"] = query.value(1);
        book["isbn"] = query.value(2);
        book["publisher_id"] = query.value(3);
        book["year"] = query.value(4);
        books.append(book);
    }

    qDebug() << "Загружено книг:" << books.size();

    return books;
}

