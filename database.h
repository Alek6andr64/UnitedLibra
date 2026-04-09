#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlQuery>

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connect();
    void disconnect();


    bool createTable();

private:
    QSqlDatabase db;
};

#endif