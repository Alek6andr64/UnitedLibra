#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connect();
    void disconnect();

    bool createTables();

    QSqlQuery selectFromTable(const QString &tableName, const QStringList &fields = {"id", "name"}, const QString &orderByField = "name");

    int addData(const QMap<QString, QVariant> &dataMap);
    bool updateData(const QMap<QString, QVariant> &updatedDataMap);
    bool updateDataAuthors(int dataId, const QVector<int> &authorIds);
    bool updateDataCategories(int dataId, const QVector<int> &categoryIds);

    QVector<int> deleteData(QVector<int> ids);
    QVector<QMap<QString, QVariant>> getData(const QString &text);

private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif