#include "sidebar.h"
#include "authdialog.h"
#include <QIcon>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>

Sidebar::Sidebar(QWidget *parent) : QFrame(parent)
{
    setupUI();
}

void Sidebar::setupUI()
{
    setFixedWidth(250);

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

    scrollContent = new QWidget();
    menuLayout = new QVBoxLayout(scrollContent);
    menuLayout->setContentsMargins(10, 15, 10, 15);
    menuLayout->setSpacing(5);

    // Секция ДАННЫЕ
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

    // Применяем стилизацию
    setupDesign();
}

void Sidebar::createHeader()
{
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(80);

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 20, 15, 10);

    titleLabel = new QLabel("UnitedLibra", headerWidget);
    titleLabel->setAlignment(Qt::AlignLeft);

    versionLabel = new QLabel("Library System v1.0", headerWidget);

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(versionLabel);

    mainLayout->addWidget(headerWidget);
}

void Sidebar::createMenuSection(const QString &title, const QVector<QPair<QString, QPair<QString, QString>>> &items)
{
    // Заголовок секции
    QLabel *sectionTitle = new QLabel(title, scrollContent);
    sectionTitles.append(sectionTitle);
    menuLayout->addWidget(sectionTitle);

    // Кнопки секции
    for (const auto &item : items) {
        QPushButton *button = createNavButton(item.first, item.second.first, item.second.second);
        menuLayout->addWidget(button);
        menuButtons.append(button);
    }

    menuLayout->addSpacing(10);
}

QPushButton* Sidebar::createNavButton(const QString &text, const QString &iconPath, const QString &signalText)
{
    QPushButton *button = new QPushButton(text, scrollContent);
    button->setProperty("signalText", signalText);
    button->setFixedHeight(40);
    button->setCursor(Qt::PointingHandCursor);

    // Загружаем иконку
    QIcon icon(iconPath);
    if (!icon.isNull()) {
        button->setIcon(icon);
        button->setIconSize(QSize(20, 20));
    }

    connect(button, &QPushButton::clicked, this, &Sidebar::onButtonClicked);

    return button;
}

void Sidebar::createAccountPanel()
{
    accountWidget = new QWidget(this);
    accountWidget->setFixedHeight(70);

    QHBoxLayout *accountLayout = new QHBoxLayout(accountWidget);
    accountLayout->setContentsMargins(15, 10, 15, 10);
    accountLayout->setSpacing(12);

    // Аватарка (кликабельная)
    avatarLabel = new QLabel(accountWidget);
    avatarLabel->setFixedSize(40, 40);
    avatarLabel->setScaledContents(true);
    avatarLabel->setCursor(Qt::PointingHandCursor);

    QPixmap avatarPixmap(":/icons/avatar.png");
    if (!avatarPixmap.isNull()) {
        avatarLabel->setPixmap(avatarPixmap);
    } else {
        avatarLabel->setText("🐱");
        avatarLabel->setAlignment(Qt::AlignCenter);
    }

    // Информация об аккаунте
    QWidget *infoWidget = new QWidget(accountWidget);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(3);

    nameLabel = new QLabel("Librarian123", infoWidget);
    roleLabel = new QLabel("Библиотекарь", infoWidget);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(roleLabel);

    accountLayout->addWidget(avatarLabel);
    accountLayout->addWidget(infoWidget);
    accountLayout->addStretch();

    mainLayout->addWidget(accountWidget);

    // Устанавливаем фильтр событий для аватарки
    avatarLabel->installEventFilter(this);
}

void Sidebar::setupDesign()
{
    // Основной стиль панели
    setStyleSheet(
        "Sidebar {"
        "    background-color: #f8f9fa;"
        "    border-right: 1px solid #dee2e6;"
        "}"
        );

    // Заголовок
    headerWidget->setStyleSheet(
        "background-color: #ffffff;"
        "border-bottom: 1px solid #dee2e6;"
        );

    titleLabel->setStyleSheet(
        "color: #212529;"
        "font-size: 18px;"
        "font-weight: bold;"
        "background-color: transparent;"
        "border: none;"
        );

    versionLabel->setStyleSheet(
        "color: #6c757d;"
        "font-size: 10px;"
        "background-color: transparent;"
        "border: none;"
        );

    // Скролл область
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: #f8f9fa;"
        "    border: none;"
        "}"
        "QScrollBar:vertical {"
        "    background-color: #f8f9fa;"
        "    width: 6px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #c0c0c0;"
        "    border-radius: 3px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background-color: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    // Заголовки секций
    for (QLabel* label : sectionTitles) {
        label->setStyleSheet(
            "color: #6c757d;"
            "font-size: 11px;"
            "font-weight: 600;"
            "padding: 8px 12px 4px 12px;"
            "background-color: transparent;"
            "border: none;"
            "letter-spacing: 0.5px;"
            );
    }

    // Кнопки меню
    for (QPushButton* button : menuButtons) {
        button->setStyleSheet(
            "QPushButton {"
            "    text-align: left;"
            "    padding: 8px 12px;"
            "    padding-left: 40px;"
            "    border: none;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: normal;"
            "    color: #495057;"
            "    background-color: transparent;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e9ecef;"
            "    color: #212529;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #dee2e6;"
            "}"
            );
    }

    // Разделитель
    QFrame *separator = scrollContent->findChild<QFrame*>();
    if (separator) {
        separator->setStyleSheet("background-color: #dee2e6;");
    }

    // Панель аккаунта
    accountWidget->setStyleSheet(
        "background-color: #ffffff;"
        "border-top: 1px solid #dee2e6;"
        );

    if (avatarLabel->text() == "🐱") {
        avatarLabel->setStyleSheet(
            "background-color: #e9ecef;"
            "border-radius: 20px;"
            "font-size: 22px;"
            );
    }

    nameLabel->setStyleSheet(
        "color: #212529;"
        "font-size: 13px;"
        "font-weight: 600;"
        "background-color: transparent;"
        "border: none;"
        );

    roleLabel->setStyleSheet(
        "color: #6c757d;"
        "font-size: 10px;"
        "background-color: transparent;"
        "border: none;"
        );
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
            "    padding: 8px 12px;"
            "    padding-left: 40px;"
            "    border: none;"
            "    border-radius: 6px;"
            "    font-size: 13px;"
            "    font-weight: normal;"
            "    color: #495057;"
            "    background-color: transparent;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e9ecef;"
            "    color: #212529;"
            "}"
            );
    }

    // Выделяем текущую кнопку
    button->setStyleSheet(
        "QPushButton {"
        "    text-align: left;"
        "    padding: 8px 12px;"
        "    padding-left: 40px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 13px;"
        "    font-weight: 600;"
        "    color: #0d6efd;"
        "    background-color: #e7f1ff;"
        "}"
        );

    currentButton = button;

    // Отправляем сигнал
    QString signalText = button->property("signalText").toString();
    emit menuItemClicked(signalText);
}

bool Sidebar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == avatarLabel && event->type() == QEvent::MouseButtonPress) {
        AuthDialog dialog(this);
        dialog.exec();
        return true;
    }
    return QFrame::eventFilter(watched, event);
}