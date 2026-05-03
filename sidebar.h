#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QHBoxLayout>

class Sidebar : public QFrame
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

signals:
    void menuItemClicked(const QString &item);

private slots:
    void onButtonClicked();

private:
    void setupUI();
    void createHeader();
    void createMenuSection(const QString &title, const QVector<QPair<QString, QPair<QString, QString>>> &items);
    void createAccountPanel();

    QPushButton* createNavButton(const QString &text, const QString &iconPath, const QString &signalText);

    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *menuLayout;

    QPushButton *currentButton;
};

#endif