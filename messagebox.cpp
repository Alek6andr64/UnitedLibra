#include "MessageBox.h"

MessageBox::MessageBox(QWidget *parent) : QWidget(parent)
{
}

int MessageBox::showInfoWithDisable(QWidget *parent, const QString &title, const QString &text, const QString &okButtonText, const QString &disableButtonText)
{
    QMessageBox msgBox(parent);

    // Устанавливаем иконку, заголовок, основной текст
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);

    // Создаем кнопки
    QPushButton *disableButton = msgBox.addButton(disableButtonText, QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(okButtonText, QMessageBox::YesRole);

    // Ставим да как ответ по умолчанию
    msgBox.setDefaultButton(okButton);

    // Выводим окно
    msgBox.exec();

    // Возвращаем результат нажатия кнопки
    if (msgBox.clickedButton() == okButton) {
        return 1;
    } else if (msgBox.clickedButton() == disableButton) {
        return 0;
    }

    return 1;
}

int MessageBox::showConfirmation(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, const QString &yesButtonText, const QString &noButtonText)
{
    QMessageBox msgBox(parent);

    // Устанавливаем иконку, заголовок, основной текст
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);

    // Если передан дополнительный текст, устанавливаем его
    if (!informativeText.isEmpty()) {
        msgBox.setInformativeText(informativeText);
    }

    // Создаем кнопки
    QPushButton *noButton = msgBox.addButton(noButtonText, QMessageBox::NoRole);
    QPushButton *yesButton = msgBox.addButton(yesButtonText, QMessageBox::YesRole);

    // Ставим да как ответ по умолчанию
    msgBox.setDefaultButton(yesButton);

    // Выводим окно
    msgBox.exec();

    // Возвращаем результат нажатия кнопки
    if (msgBox.clickedButton() == yesButton)
        return QMessageBox::Yes;
    else if (msgBox.clickedButton() == noButton)
        return QMessageBox::No;

    return QMessageBox::No;
}

void MessageBox::showInfo(QWidget *parent, const QString &title, const QString &text)
{
     // Устанавливаем иконку, заголовок, основной текст
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);

    // Выводим окно
    msgBox.exec();
}

void MessageBox::showError(QWidget *parent, const QString &title, const QString &text, const QString &informativeText)
{
    QMessageBox msgBox(parent);

    // Устанавливаем иконку, заголовок, основной текст
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);

    // Если передан дополнительный текст, устанавливаем его
    if (!informativeText.isEmpty()) {
        msgBox.setInformativeText(informativeText);
    }

    // Выводим окно
    msgBox.exec();
}