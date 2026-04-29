#ifndef DATAEDITING_H
#define DATAEDITING_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QSqlQuery>

class DataEditing : public QWidget
{
    Q_OBJECT

public:
    explicit DataEditing(QWidget *parent = nullptr);

    // Загрузить данные книги для редактирования
    void loadBookData(int bookId, const QString &title, const QString &isbn,
                      int year, int publisher_id, const QString &author = "",
                      const QString &genre = "", int copies = 0);

signals:
    void dataSaved();  // Сигнал после сохранения
    void editingFinished(); // Сигнал закрытия вкладки

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupForm();
    void setupButtons();
    bool validateInputs();
    bool saveToDatabase();

    QVBoxLayout *mainLayout;
    QGroupBox *formGroup;
    QFormLayout *formLayout;

    // Поля ввода
    QLineEdit *titleEdit;
    QLineEdit *isbnEdit;
    QSpinBox *yearSpin;
    QComboBox *publisherCombo;
    QLineEdit *authorEdit;
    QLineEdit *genreEdit;
    QSpinBox *copiesSpin;
    QLineEdit *bookIdLabel;  // Для отображения ID (только чтение)

    QPushButton *saveBtn;
    QPushButton *cancelBtn;

    int currentBookId;  // Храним ID редактируемой книги
};

#endif