#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>

class MessageBox : public QWidget
{
    Q_OBJECT

public:
    explicit MessageBox(QWidget *parent = nullptr);

    // Окно с возможностью отключения
    static int showInfoWithDisable(QWidget *parent,
        const QString &title = "Информация",
        const QString &text = "",
        const QString &okButtonText = "Ок",
        const QString &disableButtonText = "Больше не показывать");

    // Окно предупреждения
    static int showConfirmation(QWidget *parent,
        const QString &title = "Предупреждение",
        const QString &text = "",
        const QString &informativeText = "",
        const QString &yesButtonText = "Да",
        const QString &noButtonText = "Нет");

    // Окно информации
    static void showInfo(QWidget *parent,
        const QString &title = "Информация",
        const QString &text = "");

    // Окно ошибки
    static void showError(QWidget *parent,
        const QString &title = "Ошибка",
        const QString &text = "При выполнении операции произошла ошибка.",
        const QString &informativeText = "");
};

#endif