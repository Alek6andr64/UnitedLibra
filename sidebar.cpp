#include "sidebar.h"
#include "authdialog.h"
#include <QIcon>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>
#include <QStyle>

Sidebar::Sidebar(QWidget *parent) : QFrame(parent)
{
    setupUI();
}

void Sidebar::setupUI()
{
    setFixedWidth(250);
    setObjectName("sidebar");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createHeader();

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollContent = new QWidget();
    menuLayout = new QVBoxLayout(scrollContent);
    menuLayout->setContentsMargins(10, 15, 10, 15);
    menuLayout->setSpacing(5);

    QVector<QPair<QString, QPair<QString, QString>>> dataItems = {
        {"Книги", {":/resources/book.svg", "Книги"}},
        {"Выдачи", {":/resources/hand_package.svg", "Выдачи"}},
        {"Штрафы", {":/resources/request_quote.svg", "Штрафы"}},
        {"Пользователи", {":/resources/badge.svg", "Пользователи"}},
        {"Персонал", {":/resources/person_edit.svg", "Персонал"}},
        {"Логи", {":/resources/print.svg", "Логи"}}
    };
    createMenuSection("ДАННЫЕ", dataItems);

    QFrame *separator = new QFrame(scrollContent);
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(1);
    separator->setProperty("class", "separator");
    menuLayout->addWidget(separator);
    menuLayout->addSpacing(10);

    QVector<QPair<QString, QPair<QString, QString>>> settingsItems = {
        {"Вид", {":/resources/palette.svg", "Вид"}},
        {"Темы", {":/resources/keyboard.svg", "Темы"}},
        {"Аккаунт", {":/resources/settings.svg", "Аккаунт"}},
        {"Помощь", {":/resources/help.svg", "Помощь"}}
    };
    createMenuSection("НАСТРОЙКИ", settingsItems);

    menuLayout->addStretch();

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    createAccountPanel();

    headerWidget->setObjectName("headerWidget");
    titleLabel->setObjectName("titleLabel");
    versionLabel->setObjectName("versionLabel");

    accountWidget->setProperty("class", "accountPanel");
    avatarLabel->setProperty("class", "avatarLabel");
    nameLabel->setProperty("class", "nameLabel");
    roleLabel->setProperty("class", "roleLabel");

    for (QLabel* label : sectionTitles)
        label->setProperty("class", "sectionTitle");

    for (QPushButton* button : menuButtons)
        button->setProperty("class", "navButton");
}

void Sidebar::createHeader()
{
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(80);
    headerWidget->setObjectName("headerWidget");

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 20, 15, 10);

    titleLabel = new QLabel("UnitedLibra", headerWidget);
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setObjectName("titleLabel");

    versionLabel = new QLabel("Library System v1.0", headerWidget);
    versionLabel->setObjectName("versionLabel");

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(versionLabel);

    mainLayout->addWidget(headerWidget);
}

void Sidebar::createMenuSection(const QString &title, const QVector<QPair<QString, QPair<QString, QString>>> &items)
{
    QLabel *sectionTitle = new QLabel(title, scrollContent);
    sectionTitle->setProperty("class", "sectionTitle");
    sectionTitles.append(sectionTitle);
    menuLayout->addWidget(sectionTitle);

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
    button->setProperty("class", "navButton");
    button->setFixedHeight(36);
    button->setCursor(Qt::PointingHandCursor);

    QIcon icon(iconPath);
    if (!icon.isNull()) {
        button->setIcon(icon);
        button->setIconSize(QSize(18, 18));
    }

    connect(button, &QPushButton::clicked, this, &Sidebar::onButtonClicked);

    return button;
}

void Sidebar::createAccountPanel()
{
    accountWidget = new QWidget(this);
    accountWidget->setFixedHeight(70);
    accountWidget->setProperty("class", "accountPanel");

    QHBoxLayout *accountLayout = new QHBoxLayout(accountWidget);
    accountLayout->setContentsMargins(15, 10, 15, 10);
    accountLayout->setSpacing(12);

    avatarLabel = new QLabel(accountWidget);
    avatarLabel->setFixedSize(40, 40);
    avatarLabel->setScaledContents(true);
    avatarLabel->setCursor(Qt::PointingHandCursor);
    avatarLabel->setProperty("class", "avatarLabel");

    QPixmap avatarPixmap(":/resources/avatar.png");
    if (!avatarPixmap.isNull()) {
        avatarLabel->setPixmap(avatarPixmap);
    } else {
        avatarLabel->setText("👤");
        avatarLabel->setAlignment(Qt::AlignCenter);
    }

    QWidget *infoWidget = new QWidget(accountWidget);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(3);

    nameLabel = new QLabel("Librarian123", infoWidget);
    nameLabel->setProperty("class", "nameLabel");

    roleLabel = new QLabel("Библиотекарь", infoWidget);
    roleLabel->setProperty("class", "roleLabel");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(roleLabel);

    accountLayout->addWidget(avatarLabel);
    accountLayout->addWidget(infoWidget);
    accountLayout->addStretch();

    mainLayout->addWidget(accountWidget);

    avatarLabel->installEventFilter(this);
}

void Sidebar::onButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    if (currentButton) {
        currentButton->setProperty("selected", false);
        currentButton->style()->unpolish(currentButton);
        currentButton->style()->polish(currentButton);
    }

    button->setProperty("selected", true);
    button->style()->unpolish(button);
    button->style()->polish(button);

    currentButton = button;

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