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
    bool addBook(const QString &title, const QString &isbn, int publisher_id, int year);
    bool deleteBook(int id);
    QVector<QMap<QString, QVariant>> getAllBooks();

private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif