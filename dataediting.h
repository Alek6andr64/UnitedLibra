#ifndef DATAEDITING_H
#define DATAEDITING_H

#include "messagebox.h"
#include "tageditor.h"
#include "database.h"
#include "databasemodels.h"
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
    void loadData(int dataId, const QMap<QString, QVariant> &dataData);

signals:
    // Сигнал после сохранения и закрытия вкладки
    void dataSaved();
    void editingFinished();

private slots:
    // Сигнал сохранения и отмены действий
    void onSaveClicked();
    void onCancelClicked();

private:
    // Методы ответственные за создание интерфейса и инициализации логики интерфейса
    void setupUI();
    void setupForm();
    void setupButtons();
    void setupDesign();
    bool validateInputs();
    bool saveToDatabase();

    BookModel *db;

    QVBoxLayout *mainLayout;
    QGroupBox *formGroup;
    QFormLayout *formLayout;

    // Поля ввода
    QLineEdit *titleEdit;
    QLineEdit *isbnEdit;
    QSpinBox *yearSpin;
    QComboBox *publisherCombo;
    TagEditor *authorEdit;
    TagEditor *categoryEdit;
    QSpinBox *copiesSpin;
    QLineEdit *dataIdLabel;  // Для отображения ID (только чтение)

    QPushButton *saveBtn;
    QPushButton *cancelBtn;

    // Храним ID редактируемой книги
    int currentDataId = -1;
};

#endif