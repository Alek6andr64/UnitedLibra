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
    // Создаем файл library.db sqlite
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    // Вывод сообщения о подключении к бд
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
    // Если бд открыта - закрываем
    if (db.isOpen()) {
        db.close();
        qDebug() << "БД закрыта";
    }
}

bool Database::createTables()
{
    // Делаем запрос на создание таблицы
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

    // Вывод сообщения об успешности создания таблицы
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
    // Создаем запрос на добавление книги
    QSqlQuery query;
    query.prepare("INSERT INTO books (title,isbn, year, publisher_id) VALUES (:title, :isbn, :publisher_id, :year)");
    query.bindValue(":title", title);
    query.bindValue(":isbn", isbn);
    query.bindValue(":publisher_id", publisher_id);
    query.bindValue(":year", year);

    // Вывод сообщения об успешности добавления книги
    if (!query.exec()) {
        qDebug() << "Ошибка добавления:" << query.lastError().text();
        return false;
    }

    qDebug() << "Книга добавлена. ID:" << query.lastInsertId().toInt();
    return true;
}

QVector<int> Database::deleteBooks(QVector<int> ids)
{
    // Вектор для хранения id удаленных книг
    QVector<int> deletedBooksIDs;

    QSqlQuery query;
    for (int id : ids) {
        // Делаем запрос на удаление книги по id
        query.prepare("DELETE FROM books WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            // Проверяем была ли удалена хотя бы одна запись
            if (query.numRowsAffected() > 0) {
                deletedBooksIDs.append(id);
                qDebug() << "Книга с ID" << id << "успешно удалена";
            } else {
                qDebug() << "Книга с ID" << id << "не найдена в базе данных";
            }
        } else {
            qDebug() << "Ошибка удаления книги с ID" << id << ":" << query.lastError().text();
        }
    }

    qDebug() << "Всего удалено книг:" << deletedBooksIDs.size();
    return deletedBooksIDs;
}

QVector<QMap<QString, QVariant>> Database::getBooks(const QString &text)
{
    // Вектор для хранения полученных книг
    QVector<QMap<QString, QVariant>> books;

    // Готовим запрос на получение книг без фильтра и с ним
    QSqlQuery query;
    if (text.isEmpty()) {
        query.prepare("SELECT id, title, isbn, publisher_id, year FROM books ORDER BY id");
    } else {
        query.prepare("SELECT id, title, isbn, publisher_id, year FROM books "
                "WHERE id GLOB ? OR title GLOB ? OR isbn GLOB ? OR publisher_id GLOB ? OR year GLOB ? ORDER BY id;"
                );

        // Делаем учет других символов
        QString pattern = "*" + text + "*";

        // Вставляем текст из фильтров в запрос
        for (int j = 0; j < 5; j++) {
            query.addBindValue(pattern);
        }
    }

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return books;
    }

    // Заполняем ключ - значение для последующей вставки в вектор
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


