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

QSqlQuery Database::selectFromTable(const QString &tableName, const QStringList &fields, const QString &orderByField)
{
    QSqlQuery query;

    // Формируем список полей выборки, если список пуст - выбираем все поля
    QString fieldsStr = fields.isEmpty() ? "*" : fields.join(", ");

    // Делаем запрос на получение данных из таблицы
    QString sql = QString("SELECT %1 FROM %2").arg(fieldsStr, tableName);

    // Добавляем сортировку, если поле задано
    if (!orderByField.isEmpty()) {
        sql += " ORDER BY " + orderByField;
    }

    // Выполняем запрос
    if (!query.exec(sql)) {
        qDebug() << "Ошибка получения данных:" << query.lastError().text();
    }

    return query;
}


bool Database::addBook(const QString &title, const QString &isbn,  int publisher_id, int year)
{
    // Создаем запрос на добавление книги
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, isbn, year, publisher_id) VALUES (:title, :isbn, :publisher_id, :year)");
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

bool Database::updateBook(const QMap<QString, QVariant> &updatedBookData)
{
    int id = updatedBookData["id"].toInt();
    QString title = updatedBookData["title"].toString();
    QString isbn = updatedBookData["isbn"].toString();
    int year = updatedBookData["year"].toInt();
    int publisher_id = updatedBookData["publisher_id"].toInt();
    int copies = updatedBookData["copy_count"].toInt();

    QSqlQuery query;

    // Обновляем только основные данные книги
    query.prepare("UPDATE books SET title = :title, isbn = :isbn, year = :year, "
                  "publisher_id = :publisher_id WHERE id = :id");

    query.bindValue(":title", title);
    query.bindValue(":isbn", isbn);
    query.bindValue(":year", year);
    query.bindValue(":publisher_id", publisher_id);
    query.bindValue(":copies", copies);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления книги:" << query.lastError().text();
        return false;
    }

    qDebug() << "Основная информация о книге с ID" << id << "успешно обновлена";
    return true;
}

bool Database::updateBookAuthors(int bookId, const QVector<int> &authorIds)
{
    QSqlQuery query;

    // Удаляем старые связи с авторами
    query.prepare("DELETE FROM book_authors WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления старых авторов:" << query.lastError().text();
        return false;
    }

    // Добавляем новые связи с авторами
    for (int authorId : authorIds) {
        query.prepare("INSERT INTO book_authors (book_id, author_id) VALUES (:book_id, :author_id)");
        query.bindValue(":book_id", bookId);
        query.bindValue(":author_id", authorId);

        if (!query.exec()) {
            qDebug() << "Ошибка добавления автора с ID" << authorId << ":" << query.lastError().text();
            return false;
        }
    }

    qDebug() << "Авторы для книги с ID" << bookId << "успешно обновлены. Добавлено авторов:" << authorIds.size();
    return true;
}

bool Database::updateBookCategories(int bookId, const QVector<int> &categoryIds)
{
    QSqlQuery query;

    // Удаляем старые связи с жанрами
    query.prepare("DELETE FROM book_categories WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления старых жанров:" << query.lastError().text();
        return false;
    }

    // Добавляем новые связи с жанрами
    for (int categoryId : categoryIds) {
        query.prepare("INSERT INTO book_categories (book_id, category_id) VALUES (:book_id, :category_id)");
        query.bindValue(":book_id", bookId);
        query.bindValue(":category_id", categoryId);

        if (!query.exec()) {
            qDebug() << "Ошибка добавления жанра с ID" << categoryId << ":" << query.lastError().text();
            return false;
        }
    }

    qDebug() << "Жанры для книги с ID" << bookId << "успешно обновлены. Добавлено жанров:" << categoryIds.size();
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
        query.prepare(R"(
            SELECT DISTINCT
                b.id,
                b.title,
                b.isbn,
                b.year,
                b.publisher_id,
                p.name as publisher_name,
                GROUP_CONCAT(DISTINCT a.name) as authors,
                GROUP_CONCAT(DISTINCT c.name) as categories
            FROM books b
            LEFT JOIN publisher p ON b.publisher_id = p.id
            LEFT JOIN book_authors ba ON b.id = ba.book_id
            LEFT JOIN authors a ON ba.author_id = a.id
            LEFT JOIN book_categories bc ON b.id = bc.book_id
            LEFT JOIN categories c ON bc.category_id = c.id
            GROUP BY b.id
            ORDER BY b.id
        )");
    } else {
        query.prepare(R"(
            SELECT DISTINCT
                b.id,
                b.title,
                b.isbn,
                b.year,
                b.publisher_id,
                p.name as publisher_name,
                GROUP_CONCAT(DISTINCT a.name) as authors,
                GROUP_CONCAT(DISTINCT c.name) as categories
            FROM books b
            LEFT JOIN publisher p ON b.publisher_id = p.id
            LEFT JOIN book_authors ba ON b.id = ba.book_id
            LEFT JOIN authors a ON ba.author_id = a.id
            LEFT JOIN book_categories bc ON b.id = bc.book_id
            LEFT JOIN categories c ON bc.category_id = c.id
            WHERE b.id GLOB ?
               OR b.title GLOB ?
               OR b.isbn GLOB ?
               OR b.year GLOB ?
               OR p.name GLOB ?
               OR a.name GLOB ?
               OR c.name GLOB ?
            GROUP BY b.id
            ORDER BY b.id
        )");

        // Делаем учет других символов
        QString pattern = "*" + text + "*";

        // Вставляем текст из фильтров в запрос
        for (int i = 0; i < 7; ++i) {
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

        book["id"] = query.value("id");
        book["title"] = query.value("title");
        book["isbn"] = query.value("isbn");
        book["year"] = query.value("year");
        book["publisher_id"] = query.value("publisher_id");
        book["publisher_name"] = query.value("publisher_name");
        book["authors"] = query.value("authors");
        book["categories"] = query.value("categories");

        books.append(book);
    }

    qDebug() << "Загружено книг:" << books.size();

    return books;
}


