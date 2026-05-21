#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onShowRegister();
    void onShowLogin();
    void onSendCodeClicked();

private:
    void setupUI();
    void createLoginPage();
    void createRegisterPage();
    void createCodePage();
    void setupDesign();

    QStackedWidget *stackedWidget;

    // Страница входа
    QWidget *loginPage;
    QLineEdit *loginEmailEdit;
    QLineEdit *loginPasswordEdit;
    QPushButton *loginBtn;
    QPushButton *toRegisterBtn;

    // Страница регистрации
    QWidget *registerPage;
    QLineEdit *regEmailEdit;
    QLineEdit *regPasswordEdit;
    QLineEdit *regConfirmPasswordEdit;
    QPushButton *registerBtn;
    QPushButton *toLoginBtn;

    // Страница с кодом
    QWidget *codePage;
    QLineEdit *codeEdit;
    QPushButton *sendCodeBtn;
    QPushButton *backToAuthBtn;
};

#endif