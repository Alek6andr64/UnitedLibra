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

    // Заголовок
    QLabel *titleLabel = new QLabel("UnitedLibra", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(80);
    mainLayout->addWidget(titleLabel);

    // Stacked widget для переключения страниц
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    // Создаём страницы
    createLoginPage();
    createRegisterPage();
    createCodePage();

    // Начинаем со страницы входа
    stackedWidget->setCurrentWidget(loginPage);

    setupDesign();
}

void AuthDialog::createLoginPage()
{
    loginPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(50, 30, 50, 40);
    layout->setSpacing(15);

    // Заголовок страницы
    QLabel *pageTitle = new QLabel("Вход в аккаунт", loginPage);
    pageTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(pageTitle);
    layout->addSpacing(20);

    // Поле почты
    QLabel *emailLabel = new QLabel("Почта", loginPage);
    loginEmailEdit = new QLineEdit(loginPage);
    loginEmailEdit->setPlaceholderText("example@mail.com");
    loginEmailEdit->setFixedHeight(35);
    layout->addWidget(emailLabel);
    layout->addWidget(loginEmailEdit);
    layout->addSpacing(10);

    // Поле пароля
    QLabel *passwordLabel = new QLabel("Пароль", loginPage);
    loginPasswordEdit = new QLineEdit(loginPage);
    loginPasswordEdit->setPlaceholderText("••••••••");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setFixedHeight(35);
    layout->addWidget(passwordLabel);
    layout->addWidget(loginPasswordEdit);

    layout->addStretch();

    // Ссылка на регистрацию
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

    // Кнопка входа
    loginBtn = new QPushButton("Войти", loginPage);
    loginBtn->setFixedHeight(40);
    layout->addWidget(loginBtn);

    // Подключаем сигналы
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

    // Заголовок страницы
    QLabel *pageTitle = new QLabel("Регистрация аккаунта", registerPage);
    pageTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(pageTitle);
    layout->addSpacing(20);

    // Поле почты
    QLabel *emailLabel = new QLabel("Почта", registerPage);
    regEmailEdit = new QLineEdit(registerPage);
    regEmailEdit->setPlaceholderText("example@mail.com");
    regEmailEdit->setFixedHeight(35);
    layout->addWidget(emailLabel);
    layout->addWidget(regEmailEdit);
    layout->addSpacing(10);

    // Поле пароля
    QLabel *passwordLabel = new QLabel("Пароль", registerPage);
    regPasswordEdit = new QLineEdit(registerPage);
    regPasswordEdit->setPlaceholderText("••••••••");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setFixedHeight(35);
    layout->addWidget(passwordLabel);
    layout->addWidget(regPasswordEdit);
    layout->addSpacing(10);

    // Поле подтверждения пароля
    QLabel *confirmLabel = new QLabel("Повт пароль", registerPage);
    regConfirmPasswordEdit = new QLineEdit(registerPage);
    regConfirmPasswordEdit->setPlaceholderText("••••••••");
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setFixedHeight(35);
    layout->addWidget(confirmLabel);
    layout->addWidget(regConfirmPasswordEdit);

    layout->addStretch();

    // Ссылка на вход
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

    // Кнопка регистрации
    registerBtn = new QPushButton("Зарегистрироваться", registerPage);
    registerBtn->setFixedHeight(40);
    layout->addWidget(registerBtn);

    // Подключаем сигналы
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

    // Заголовок
    QLabel *pageTitle = new QLabel("Вам на почту пришел код", codePage);
    pageTitle->setAlignment(Qt::AlignCenter);
    pageTitle->setWordWrap(true);
    layout->addWidget(pageTitle);
    layout->addSpacing(30);

    // Поле для кода
    QLabel *codeLabel = new QLabel("Код подтверждения", codePage);
    codeEdit = new QLineEdit(codePage);
    codeEdit->setPlaceholderText("Введите код из письма");
    codeEdit->setFixedHeight(35);
    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);

    layout->addStretch();

    // Кнопка назад
    backToAuthBtn = new QPushButton("← Вернуться к входу", codePage);
    backToAuthBtn->setFlat(true);
    backToAuthBtn->setFixedHeight(35);
    layout->addWidget(backToAuthBtn);
    layout->addSpacing(10);

    // Кнопка отправки
    sendCodeBtn = new QPushButton("Отправить", codePage);
    sendCodeBtn->setFixedHeight(40);
    layout->addWidget(sendCodeBtn);

    // Подключаем сигналы
    connect(backToAuthBtn, &QPushButton::clicked, this, &AuthDialog::onShowLogin);
    connect(sendCodeBtn, &QPushButton::clicked, this, &AuthDialog::onSendCodeClicked);

    stackedWidget->addWidget(codePage);
}

void AuthDialog::setupDesign()
{
    setStyleSheet(
        "QDialog {"
        "    background-color: #ffffff;"
        "}"
        "QLabel {"
        "    color: #212529;"
        "    font-size: 13px;"
        "}"
        "QLineEdit {"
        "    padding: 8px 12px;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 8px;"
        "    font-size: 13px;"
        "    background-color: #ffffff;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #0d6efd;"
        "    outline: none;"
        "}"
        "QPushButton {"
        "    padding: 8px 16px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "}"
        "QPushButton[flat=\"true\"] {"
        "    background-color: transparent;"
        "    color: #0d6efd;"
        "}"
        "QPushButton[flat=\"true\"]:hover {"
        "    color: #0a58ca;"
        "}"
        );

    // Стиль для главного заголовка
    QLabel *mainTitle = findChild<QLabel*>();
    if (mainTitle && mainTitle->text() == "UnitedLibra") {
        mainTitle->setStyleSheet(
            "font-size: 26px;"
            "font-weight: bold;"
            "color: #0d6efd;"
            "background-color: #f8f9fa;"
            "border-bottom: 1px solid #dee2e6;"
            );
    }

    // Стиль для кнопки входа
    loginBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #0d6efd;"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b5ed7;"
        "}"
        );

    // Стиль для кнопки регистрации
    registerBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #0d6efd;"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b5ed7;"
        "}"
        );

    // Стиль для кнопки отправки кода
    sendCodeBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #0d6efd;"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b5ed7;"
        "}"
        );

    // Стиль для заголовков страниц
    QList<QLabel*> pageTitles = findChildren<QLabel*>();
    for (QLabel* label : pageTitles) {
        if (label->text() == "Вход в аккаунт" ||
            label->text() == "Регистрация аккаунта") {
            label->setStyleSheet(
                "font-size: 22px;"
                "font-weight: bold;"
                "color: #212529;"
                );
        }
        if (label->text() == "Вам на почту пришел код") {
            label->setStyleSheet(
                "font-size: 18px;"
                "font-weight: bold;"
                "color: #212529;"
                );
        }
    }

    // Стиль для ссылок
    toRegisterBtn->setStyleSheet(
        "QPushButton {"
        "    color: #0d6efd;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"
        "}"
        );

    toLoginBtn->setStyleSheet(
        "QPushButton {"
        "    color: #0d6efd;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"
        "}"
        );

    backToAuthBtn->setStyleSheet(
        "QPushButton {"
        "    color: #6c757d;"
        "    font-size: 13px;"
        "    text-align: left;"
        "}"
        "QPushButton:hover {"
        "    color: #0d6efd;"
        "}"
        );
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
    // Проверка паролей
    if (regPasswordEdit->text() != regConfirmPasswordEdit->text()) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают!");
        return;
    }

    // Переходим на страницу с кодом
    stackedWidget->setCurrentWidget(codePage);
}

void AuthDialog::onSendCodeClicked()
{
    QMessageBox::information(this, "Информация", "Код подтверждён! Теперь можно войти.");
    accept();
}