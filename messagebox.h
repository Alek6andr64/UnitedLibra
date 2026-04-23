#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

class MessageBox : public QWidget
{
    Q_OBJECT

public:
    explicit MessageBox(QWidget *parent = nullptr);

    // Метод отображения предупреждения об обновлении данных
    static int showUpdateInfo(QWidget *parent);

    // Метод отображения предупреждения об удалении данных
    static int showDeleteConfirmation(QWidget *parent);

    // Метод отображения ошибки при удалении данных
    static void showDeleteError(QWidget *parent, int booksDeleted, int booksToDelete);

};

#endif
