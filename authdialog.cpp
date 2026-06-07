#include "authdialog.h"
#include <QMessageBox>
#include <QSpacerItem>

AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent)
{
    setupUI();
}

void AuthDialog::setupUI()
{
    setFixedSize(400, 550);
    setWindowTitle("UnitedLibra - Вход в аккаунт");
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLabel *titleLabel = new QLabel("UnitedLibra", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(80);
    mainLayout->addWidget(titleLabel);

    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    createLoginPage();
    createRegisterPage();
    createCodePage();

    stackedWidget->setCurrentWidget(loginPage);
}

void AuthDialog::createLoginPage()
{
    loginPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(50, 30, 50, 40);
    layout->setSpacing(15);

    QLabel *pageTitle = new QLabel("Вход в аккаунт", loginPage);
    pageTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(pageTitle);
    layout->addSpacing(20);

    QLabel *emailLabel = new QLabel("Почта", loginPage);
    loginEmailEdit = new QLineEdit(loginPage);
    loginEmailEdit->setPlaceholderText("example@mail.com");
    loginEmailEdit->setFixedHeight(35);
    layout->addWidget(emailLabel);
    layout->addWidget(loginEmailEdit);
    layout->addSpacing(10);

    QLabel *passwordLabel = new QLabel("Пароль", loginPage);
    loginPasswordEdit = new QLineEdit(loginPage);
    loginPasswordEdit->setPlaceholderText("••••••••");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setFixedHeight(35);
    layout->addWidget(passwordLabel);
    layout->addWidget(loginPasswordEdit);

    layout->addStretch();

    QHBoxLayout *registerLinkLayout = new QHBoxLayout();
    registerLinkLayout->addStretch();
    QLabel *noAccountLabel = new QLabel("Нет аккаунта?", loginPage);
    toRegisterBtn = new QPushButton("Зарегистрироваться", loginPage);
    toRegisterBtn->setFlat(true);
    registerLinkLayout->addWidget(noAccountLabel);
    registerLinkLayout->addWidget(toRegisterBtn);
    registerLinkLayout->addStretch();
    layout->addLayout(registerLinkLayout);
    layout->addSpacing(10);

    loginBtn = new QPushButton("Войти", loginPage);
    loginBtn->setFixedHeight(40);
    layout->addWidget(loginBtn);

    connect(toRegisterBtn, &QPushButton::clicked, this, &AuthDialog::onShowRegister);
    connect(loginBtn, &QPushButton::clicked, this, &AuthDialog::onLoginClicked);

    stackedWidget->addWidget(loginPage);
}

void AuthDialog::createRegisterPage()
{
    registerPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(registerPage);
    layout->setContentsMargins(50, 30, 50, 40);
    layout->setSpacing(15);

    QLabel *pageTitle = new QLabel("Регистрация аккаунта", registerPage);
    pageTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(pageTitle);
    layout->addSpacing(20);

    QLabel *emailLabel = new QLabel("Почта", registerPage);
    regEmailEdit = new QLineEdit(registerPage);
    regEmailEdit->setPlaceholderText("example@mail.com");
    regEmailEdit->setFixedHeight(35);
    layout->addWidget(emailLabel);
    layout->addWidget(regEmailEdit);
    layout->addSpacing(10);

    QLabel *passwordLabel = new QLabel("Пароль", registerPage);
    regPasswordEdit = new QLineEdit(registerPage);
    regPasswordEdit->setPlaceholderText("••••••••");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setFixedHeight(35);
    layout->addWidget(passwordLabel);
    layout->addWidget(regPasswordEdit);
    layout->addSpacing(10);

    QLabel *confirmLabel = new QLabel("Повт пароль", registerPage);
    regConfirmPasswordEdit = new QLineEdit(registerPage);
    regConfirmPasswordEdit->setPlaceholderText("••••••••");
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setFixedHeight(35);
    layout->addWidget(confirmLabel);
    layout->addWidget(regConfirmPasswordEdit);

    layout->addStretch();

    QHBoxLayout *loginLinkLayout = new QHBoxLayout();
    loginLinkLayout->addStretch();
    QLabel *haveAccountLabel = new QLabel("Есть аккаунт?", registerPage);
    toLoginBtn = new QPushButton("Войти", registerPage);
    toLoginBtn->setFlat(true);
    loginLinkLayout->addWidget(haveAccountLabel);
    loginLinkLayout->addWidget(toLoginBtn);
    loginLinkLayout->addStretch();
    layout->addLayout(loginLinkLayout);
    layout->addSpacing(10);

    registerBtn = new QPushButton("Зарегистрироваться", registerPage);
    registerBtn->setFixedHeight(40);
    layout->addWidget(registerBtn);

    connect(toLoginBtn, &QPushButton::clicked, this, &AuthDialog::onShowLogin);
    connect(registerBtn, &QPushButton::clicked, this, &AuthDialog::onRegisterClicked);

    stackedWidget->addWidget(registerPage);
}

void AuthDialog::createCodePage()
{
    codePage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(codePage);
    layout->setContentsMargins(50, 30, 50, 40);
    layout->setSpacing(15);

    QLabel *pageTitle = new QLabel("Вам на почту пришел код", codePage);
    pageTitle->setAlignment(Qt::AlignCenter);
    pageTitle->setWordWrap(true);
    layout->addWidget(pageTitle);
    layout->addSpacing(30);

    QLabel *codeLabel = new QLabel("Код подтверждения", codePage);
    codeEdit = new QLineEdit(codePage);
    codeEdit->setPlaceholderText("Введите код из письма");
    codeEdit->setFixedHeight(35);
    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);

    layout->addStretch();

    backToAuthBtn = new QPushButton("← Вернуться к входу", codePage);
    backToAuthBtn->setFlat(true);
    backToAuthBtn->setFixedHeight(35);
    layout->addWidget(backToAuthBtn);
    layout->addSpacing(10);

    sendCodeBtn = new QPushButton("Отправить", codePage);
    sendCodeBtn->setFixedHeight(40);
    layout->addWidget(sendCodeBtn);

    connect(backToAuthBtn, &QPushButton::clicked, this, &AuthDialog::onShowLogin);
    connect(sendCodeBtn, &QPushButton::clicked, this, &AuthDialog::onSendCodeClicked);

    stackedWidget->addWidget(codePage);
}

void AuthDialog::onShowRegister()
{
    stackedWidget->setCurrentWidget(registerPage);
}

void AuthDialog::onShowLogin()
{
    stackedWidget->setCurrentWidget(loginPage);
}

void AuthDialog::onLoginClicked()
{
    QMessageBox::information(this, "Информация", "Функция входа в разработке");
}

void AuthDialog::onRegisterClicked()
{
    if (regPasswordEdit->text() != regConfirmPasswordEdit->text()) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        return;
    }

    stackedWidget->setCurrentWidget(codePage);
}

void AuthDialog::onSendCodeClicked()
{
    QMessageBox::information(this, "Информация", "Код подтверждён! Теперь можно войти.");
    accept();
}