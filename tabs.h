#ifndef TABS_H
#define TABS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>

class Tabs : public QWidget
{
    Q_OBJECT

public:
    explicit Tabs(QWidget *parent = nullptr);
    QTabWidget *tabWidget; // Пространство вкладок

private:
    void setupUI();

    QHBoxLayout *tabsLayout;
};

#endif
