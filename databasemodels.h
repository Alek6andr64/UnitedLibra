#ifndef DATABASEMODELS_H
#define DATABASEMODELS_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QDebug>

class DataModel : public QObject
{
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~DataModel() {}

    // Методы для работы с данными
    virtual int addData(const QMap<QString, QVariant> &dataMap) = 0;
    virtual bool updateData(const QMap<QString, QVariant> &updatedDataMap) = 0;
    virtual QVector<int> deleteData(QVector<int> ids) = 0;
    virtual QVector<QMap<QString, QVariant>> getData(const QString &text = "") = 0;

    // Метод для выборки из любых таблиц
    QSqlQuery selectFromTable(const QString &tableName, const QStringList &fields = {"id", "name"}, const QString &orderByField = "name");

protected:
    QSqlDatabase db;
    virtual QString getTableName() const = 0;
    virtual QStringList getFields() const = 0;
    virtual QString getSearchQuery() const = 0;
    virtual QMap<QString, QVariant> recordToMap(const QSqlQuery &query) const = 0;

    // Метод для получения имени подключения
    virtual QString getConnectionName() const = 0;
    void ensureConnection();
};

class BookModel : public DataModel
{
    Q_OBJECT

public:
    explicit BookModel(QObject *parent = nullptr);
    ~BookModel();

    // Реализация виртуальных методов DataModel
    int addData(const QMap<QString, QVariant> &dataMap) override;
    bool updateData(const QMap<QString, QVariant> &updatedDataMap) override;
    QVector<int> deleteData(QVector<int> ids) override;
    QVector<QMap<QString, QVariant>> getData(const QString &text = "") override;

    // Специфичные методы для книг
    bool updateBookAuthors(int bookId, const QVector<int> &authorIds);
    bool updateBookCategories(int bookId, const QVector<int> &categoryIds);
    bool createTables();

protected:
    QString getTableName() const override;
    QStringList getFields() const override;
    QString getSearchQuery() const override;
    QMap<QString, QVariant> recordToMap(const QSqlQuery &query) const override;
    QString getConnectionName() const override;

private:
    QString connectionName;
};

#endif