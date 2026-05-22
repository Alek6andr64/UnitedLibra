#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QHBoxLayout>
#include <QEvent>

class Sidebar : public QFrame
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

signals:
    void menuItemClicked(const QString &item);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onButtonClicked();

private:
    void setupUI();
    void createHeader();
    void createMenuSection(const QString &title, const QVector<QPair<QString, QPair<QString, QString>>> &items);
    void createAccountPanel();
    void setupDesign();

    QPushButton* createNavButton(const QString &text, const QString &iconPath, const QString &signalText);

    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *menuLayout;

    // Виджеты для стилизации
    QWidget *headerWidget;
    QLabel *titleLabel;
    QLabel *versionLabel;
    QWidget *accountWidget;
    QLabel *avatarLabel;
    QLabel *nameLabel;
    QLabel *roleLabel;
    QVector<QLabel*> sectionTitles;
    QVector<QPushButton*> menuButtons;

    QPushButton *currentButton;
};

#endif