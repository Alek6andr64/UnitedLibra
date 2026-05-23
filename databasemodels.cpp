#include "databasemodels.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void DataModel::ensureConnection() {
    QString connName = getConnectionName();

    if (!QSqlDatabase::contains(connName)) {
        // Создаем соединение с БД для этой модели
        QSqlDatabase newDb = QSqlDatabase::addDatabase("QSQLITE", connName);
        newDb.setDatabaseName("library.db");

        if (!newDb.open()) {
            qDebug() << "Ошибка подключения к БД для модели:" << connName << newDb.lastError().text();
        }
    }

    db = QSqlDatabase::database(connName);
}

QSqlQuery DataModel::selectFromTable(const QString &tableName,const QStringList &fields, const QString &orderByField){
    QSqlQuery query(db);

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
        qDebug() << "Ошибка получения данных из таблицы" << tableName << ":" << query.lastError().text();
    }

    return query;
}



BookModel::BookModel(QObject *parent) : DataModel(parent)
{
    connectionName = "books_connection";
    ensureConnection();
}


BookModel::~BookModel(){

}

QString BookModel::getConnectionName() const
{
    return connectionName;
}

QString BookModel::getTableName() const
{
    return "books";
}

QStringList BookModel::getFields() const
{
    return {"id", "title", "isbn", "year", "publisher_id"};
}

QString BookModel::getSearchQuery() const
{
    return R"(
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
    )";
}

QMap<QString, QVariant> BookModel::recordToMap(const QSqlQuery &query) const
{
    QMap<QString, QVariant> dataItem;
    dataItem["id"] = query.value("id");
    dataItem["title"] = query.value("title");
    dataItem["isbn"] = query.value("isbn");
    dataItem["year"] = query.value("year");
    dataItem["publisher_id"] = query.value("publisher_id");
    dataItem["publisher_name"] = query.value("publisher_name");
    dataItem["authors"] = query.value("authors");
    dataItem["categories"] = query.value("categories");
    return dataItem;
}

bool BookModel::createTables()
{
    QSqlQuery query(db);

    // Создание таблицы books
    QString createBooksQuery = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            isbn TEXT NOT NULL,
            year INTEGER,
            publisher_id INTEGER NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createBooksQuery)) {
        qDebug() << "Ошибка создания таблицы books:" << query.lastError().text();
        return false;
    }

    qDebug() << "Таблица 'books' создана/существует";
    return true;
}

int BookModel::addData(const QMap<QString, QVariant> &dataMap)
{
    QString title = dataMap["title"].toString();
    QString isbn = dataMap["isbn"].toString();
    int publisher_id = dataMap["publisher_id"].toInt();
    int year = dataMap["year"].toInt();

    // Создаем запрос на добавление книги
    QSqlQuery query(db);
    query.prepare("INSERT INTO books (title, isbn, year, publisher_id) VALUES (:title, :isbn, :year, :publisher_id)");
    query.bindValue(":title", title);
    query.bindValue(":isbn", isbn);
    query.bindValue(":year", year);
    query.bindValue(":publisher_id", publisher_id);

    // Вывод сообщения об успешности добавления
    if (!query.exec()) {
        qDebug() << "Ошибка добавления книги:" << query.lastError().text();
        return -1;
    }

    qDebug() << "Книга добавлена. ID:" << query.lastInsertId().toInt();
    return query.lastInsertId().toInt();
}

bool BookModel::updateData(const QMap<QString, QVariant> &updatedDataMap)
{
    int id = updatedDataMap["id"].toInt();
    QString title = updatedDataMap["title"].toString();
    QString isbn = updatedDataMap["isbn"].toString();
    int year = updatedDataMap["year"].toInt();
    int publisher_id = updatedDataMap["publisher_id"].toInt();

    QSqlQuery query(db);

    // Обновляем данные книги
    query.prepare("UPDATE books SET title = :title, isbn = :isbn, year = :year, "
                  "publisher_id = :publisher_id WHERE id = :id");

    query.bindValue(":title", title);
    query.bindValue(":isbn", isbn);
    query.bindValue(":year", year);
    query.bindValue(":publisher_id", publisher_id);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления книги:" << query.lastError().text();
        return false;
    }

    qDebug() << "Основная информация о книге с ID" << id << "успешно обновлена";
    return true;
}

bool BookModel::updateBookAuthors(int bookId, const QVector<int> &authorIds)
{
    QSqlQuery query(db);

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

bool BookModel::updateBookCategories(int bookId, const QVector<int> &categoryIds)
{
    QSqlQuery query(db);

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

QVector<int> BookModel::deleteData(QVector<int> ids)
{
    QVector<int> deletedDataIDs;
    QSqlQuery query(db);

    for (int id : ids) {
        query.prepare("DELETE FROM books WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            if (query.numRowsAffected() > 0) {
                deletedDataIDs.append(id);
                qDebug() << "Книга с ID" << id << "успешно удалена";
            } else {
                qDebug() << "Книга с ID" << id << "не найдена в базе данных";
            }
        } else {
            qDebug() << "Ошибка удаления книги с ID" << id << ":" << query.lastError().text();
        }
    }

    qDebug() << "Всего удалено книг:" << deletedDataIDs.size();
    return deletedDataIDs;
}

QVector<QMap<QString, QVariant>> BookModel::getData(const QString &text)
{
    QVector<QMap<QString, QVariant>> allData;
    QSqlQuery query(db);

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
        query.prepare(getSearchQuery());

        QString pattern = "*" + text + "*";

        // Вставляем текст из фильтров в запрос
        for (int i = 0; i < 7; ++i) {
            query.addBindValue(pattern);
        }
    }

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return allData;
    }

    while (query.next()) {
        allData.append(recordToMap(query));
    }

    qDebug() << "Загружено книг:" << allData.size();
    return allData;
}