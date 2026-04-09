#include "database.h"

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

    // надо создать отлов ошибок

    return true;
}

void Database::disconnect()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "БД закрыта";
    }
}

bool Database::createTable()
{
    QSqlQuery query;

    QString createQuery = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            year INTEGER,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";

    // надо создать отлов ошибок

    return true;
}
