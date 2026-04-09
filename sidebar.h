#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QVBoxLayout>

class Sidebar : public QFrame
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

private:
    void setupUI();

    QVBoxLayout *mainLayout; // Основная область сайдбара

};

#endif