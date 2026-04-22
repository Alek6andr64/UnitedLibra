#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>

class Sidebar : public QFrame
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

signals:
    void menuItemClicked(const QString &item);

private slots:
    void onTreeClicked(const QModelIndex &index);

private:
    void setupUI();
    void setupTree();

    QVBoxLayout *mainLayout;
    QTreeView *treeView;
    QStandardItemModel *model;
};

#endif