#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QDebug>

#include "databasemodels.h"

// Enum для типов данных
enum class DataModelType {
    Books,      // Книги
    Authors,    // Авторы
    Publishers, // Издатели
    BookCopies, // Копии книг
    Loans,      // Выдачи
    Fines,      // Штрафы
    Users       // Пользователи
};

class DatabaseFactory
{
public:
    // Метод создания модели по типу
    static DataModel* createModel(DataModelType type, QObject *parent = nullptr);

    // Методы для подключения к базе данных
    static bool connectDatabase(const QString &connectionName = "default");
    static void disconnectDatabase(const QString &connectionName = "default");
};

#endif