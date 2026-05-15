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

    bool addBook(const QString &title, const QString &isbn, int publisher_id, int year);
    bool updateBook(const QMap<QString, QVariant> &updatedBookData);
    bool updateBookAuthors(int bookId, const QVector<int> &authorIds);
    bool updateBookCategories(int bookId, const QVector<int> &categoryIds);

    QVector<int> deleteBooks(QVector<int> ids);
    QVector<QMap<QString, QVariant>> getBooks(const QString &text);


private:
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif