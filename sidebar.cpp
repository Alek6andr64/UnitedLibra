#include "sidebar.h"
#include <QIcon>
#include <QPixmap>

Sidebar::Sidebar(QWidget *parent) : QFrame(parent)
{
    setupUI();
}

void Sidebar::setupUI()
{
    setFixedWidth(250);
    setStyleSheet(
        "Sidebar {"
        "    background-color: #4a148c;"
        "    border: none;"
        "}"
        );

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Заголовок
    createHeader();

    // Скролл область для меню
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: #4a148c;"
        "    border: none;"
        "}"
        "QScrollBar:vertical {"
        "    background-color: #4a148c;"
        "    width: 8px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #311b6b;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background-color: #6a1b9a;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    scrollContent = new QWidget();
    menuLayout = new QVBoxLayout(scrollContent);
    menuLayout->setContentsMargins(10, 15, 10, 15);
    menuLayout->setSpacing(5);

    // Секция ДАННЫЕ
    // Формат: {Текст на кнопке, {путь к иконке, сигнал}}
    QVector<QPair<QString, QPair<QString, QString>>> dataItems = {
        {"Книги", {":/icons/books.png", "Книги"}},
        {"Выдачи", {":/icons/loans.png", "Выдачи"}},
        {"Штрафы", {":/icons/fines.png", "Штрафы"}},
        {"Пользователи", {":/icons/users.png", "Пользователи"}},
        {"Персонал", {":/icons/staff.png", "Персонал"}},
        {"Логи", {":/icons/logs.png", "Логи"}}
    };
    createMenuSection("ДАННЫЕ", dataItems);

    // Разделитель
    QFrame *separator = new QFrame(scrollContent);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("background-color: #311b6b; max-height: 1px;");
    separator->setFixedHeight(1);
    menuLayout->addWidget(separator);
    menuLayout->addSpacing(10);

    // Секция НАСТРОЙКИ
    QVector<QPair<QString, QPair<QString, QString>>> settingsItems = {
        {"Вид", {":/icons/appearance.png", "Вид"}},
        {"Темы", {":/icons/themes.png", "Темы"}},
        {"Аккаунт", {":/icons/account.png", "Аккаунт"}},
        {"Помощь", {":/icons/help.png", "Помощь"}}
    };
    createMenuSection("НАСТРОЙКИ", settingsItems);

    menuLayout->addStretch();

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Панель аккаунта
    createAccountPanel();
}

void Sidebar::createHeader()
{
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(80);
    headerWidget->setStyleSheet("background-color: #311b6b;");

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 20, 15, 10);

    QLabel *titleLabel = new QLabel("UnitedLibra", headerWidget);
    titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        "font-family: 'Segoe UI';"
        "background-color: transparent;"
        "border: none;"
        );
    titleLabel->setAlignment(Qt::AlignLeft);

    QLabel *versionLabel = new QLabel("Library System v1.0", headerWidget);
    versionLabel->setStyleSheet(
        "color: #aa88ff;"
        "font-size: 10px;"
        "background-color: transparent;"
        "border: none;"
        );

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(versionLabel);

    mainLayout->addWidget(headerWidget);
}

void Sidebar::createMenuSection(const QString &title, const QVector<QPair<QString, QPair<QString, QString>>> &items)
{
    // Заголовок секции
    QLabel *sectionTitle = new QLabel(title, scrollContent);
    sectionTitle->setStyleSheet(
        "color: #aa88ff;"
        "font-size: 11px;"
        "font-weight: bold;"
        "padding: 8px 12px 4px 12px;"
        "background-color: transparent;"
        "border: none;"
        "letter-spacing: 1px;"
        );
    menuLayout->addWidget(sectionTitle);

    // Кнопки секции
    for (const auto &item : items) {
        QPushButton *button = createNavButton(item.first, item.second.first, item.second.second);
        menuLayout->addWidget(button);
    }

    menuLayout->addSpacing(10);
}

QPushButton* Sidebar::createNavButton(const QString &text, const QString &iconPath, const QString &signalText)
{
    QPushButton *button = new QPushButton(text, scrollContent);
    button->setProperty("signalText", signalText);
    button->setFixedHeight(45);
    button->setCursor(Qt::PointingHandCursor);

    // Загружаем иконку
    QIcon icon(iconPath);
    if (!icon.isNull()) {
        button->setIcon(icon);
        button->setIconSize(QSize(22, 22));
    }

    button->setStyleSheet(
        "QPushButton {"
        "    text-align: left;"
        "    padding: 10px 12px;"
        "    padding-left: 42px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "    color: #e0e0e0;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: #311b6b;"
        "    color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6a1b9a;"
        "}"
        );

    connect(button, &QPushButton::clicked, this, &Sidebar::onButtonClicked);

    return button;
}

void Sidebar::createAccountPanel()
{
    QWidget *accountWidget = new QWidget(this);
    accountWidget->setFixedHeight(80);
    accountWidget->setStyleSheet(
        "background-color: #311b6b;"
        "border-top: 1px solid #6a1b9a;"
        );

    QHBoxLayout *accountLayout = new QHBoxLayout(accountWidget);
    accountLayout->setContentsMargins(15, 10, 15, 10);
    accountLayout->setSpacing(12);

    // Аватарка (из PNG)
    QLabel *avatarLabel = new QLabel(accountWidget);
    avatarLabel->setFixedSize(45, 45);
    avatarLabel->setScaledContents(true);

    QPixmap avatarPixmap(":/icons/avatar.png");
    if (!avatarPixmap.isNull()) {
        avatarLabel->setPixmap(avatarPixmap);
    } else {
        // Если картинка не загрузилась, показываем котика эмодзи
        avatarLabel->setText("🐱");
        avatarLabel->setStyleSheet("font-size: 28px; background-color: #6a1b9a; border-radius: 22px;");
        avatarLabel->setAlignment(Qt::AlignCenter);
    }

    // Информация об аккаунте
    QWidget *infoWidget = new QWidget(accountWidget);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(3);

    QLabel *nameLabel = new QLabel("Librarian123", infoWidget);
    nameLabel->setStyleSheet(
        "color: white;"
        "font-size: 13px;"
        "font-weight: bold;"
        "background-color: transparent;"
        "border: none;"
        );

    QLabel *roleLabel = new QLabel("Библиотекарь", infoWidget);
    roleLabel->setStyleSheet(
        "color: #aa88ff;"
        "font-size: 10px;"
        "background-color: transparent;"
        "border: none;"
        );

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(roleLabel);

    accountLayout->addWidget(avatarLabel);
    accountLayout->addWidget(infoWidget);
    accountLayout->addStretch();

    mainLayout->addWidget(accountWidget);
}

void Sidebar::onButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    // Сбрасываем стиль предыдущей кнопки
    if (currentButton) {
        currentButton->setStyleSheet(
            "QPushButton {"
            "    text-align: left;"
            "    padding: 10px 12px;"
            "    padding-left: 42px;"
            "    border: none;"
            "    border-radius: 8px;"
            "    font-size: 13px;"
            "    font-weight: 500;"
            "    color: #e0e0e0;"
            "    background-color: transparent;"
            "}"
            "QPushButton:hover {"
            "    background-color: #311b6b;"
            "    color: white;"
            "}"
            );
    }

    // Выделяем текущую кнопку
    button->setStyleSheet(
        "QPushButton {"
        "    text-align: left;"
        "    padding: 10px 12px;"
        "    padding-left: 42px;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "    color: white;"
        "    background-color: #6a1b9a;"
        "}"
        );

    currentButton = button;

    // Отправляем сигнал с чистым текстом из свойства
    QString signalText = button->property("signalText").toString();
    emit menuItemClicked(signalText);
}