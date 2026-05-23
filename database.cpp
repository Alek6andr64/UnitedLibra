#include "database.h"
#include "databasemodels.h"
#include <QSqlQuery>
#include <QSqlError>

bool DatabaseFactory::connectDatabase(const QString &connectionName) {
    // Создаем подключение к базе данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName("library.db");

    // Вывод сообщения о подключении к бд
    bool success = db.open();
    if (success) {
        qDebug() << "Вы успешно подключились к базе данных:" << db.databaseName();
    } else {
        qDebug() << "При подключении к базе данных произошла ошибка:" << db.lastError().databaseText();
    }

    return success;
}

void DatabaseFactory::disconnectDatabase(const QString &connectionName) {
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (db.isOpen()) {
            db.close();
            qDebug() << "БД закрыта (подключение:" << connectionName << ")";
        }
        QSqlDatabase::removeDatabase(connectionName);
    }
}

DataModel* DatabaseFactory::createModel(DataModelType type, QObject *parent) {
    switch (type) {
    case DataModelType::Books:
        return new BookModel(parent);

    default:
        qDebug() << "Неизвестный тип модели";
        return nullptr;
    }
}