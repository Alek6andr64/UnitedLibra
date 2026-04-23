#include "MessageBox.h"

MessageBox::MessageBox(QWidget *parent): QWidget(parent){

}

int MessageBox::showUpdateInfo(QWidget *parent)
{
    // Создаем окно ошибки при удалении с текстом
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Информация");
    msgBox.setText("Данные были обновлены");

    QPushButton *doNotShowButton = msgBox.addButton("Больше не показывать", QMessageBox::ActionRole);
    QPushButton *yesButton = msgBox.addButton("Ок", QMessageBox::YesRole);

    // Ставим "Да" как ответ по умолчанию
    msgBox.setDefaultButton(yesButton);

    // Выводим окно
    msgBox.exec();

    // Возвращаем результат нажатия кнопки
    if (msgBox.clickedButton() == yesButton) {
        return 1;
    }
    else if (msgBox.clickedButton() == doNotShowButton) {
        return 0;
    }
    // Значение по умолчанию
    return QMessageBox::Yes;

}

int MessageBox::showDeleteConfirmation(QWidget *parent)
{
    // Создаем окно предупреждения об удалении с текстом
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Предупреждение");
    msgBox.setText("Вы действительно хотите удалить выбранные записи?");
    msgBox.setInformativeText("Это действие нельзя отменить.");

    // Определяем кнопки
    QPushButton *noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
    QPushButton *yesButton = msgBox.addButton("Да", QMessageBox::YesRole);

    // Ставим "Да" как ответ по умолчанию
    msgBox.setDefaultButton(yesButton);

    // Выводим окно
    msgBox.exec();

    // Возвращаем результат нажатия кнопки
    if (msgBox.clickedButton() == yesButton)
        return QMessageBox::Yes;
    else if (msgBox.clickedButton() == noButton)
        return QMessageBox::No;

    // Значение по умолчанию
    return QMessageBox::No;
}

void MessageBox::showDeleteError(QWidget *parent, int booksDeleted, int booksToDelete)
{
    // Создаем окно ошибки при удалении с текстом
    QMessageBox msgBox(parent);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Ошибка");
    msgBox.setText("При удалении данных произошла неизвестная ошибка.");
    msgBox.setInformativeText(QString("%1 книг из %2 удалено").arg(booksDeleted).arg(booksToDelete));

    // Выводим окно
    msgBox.exec();

}