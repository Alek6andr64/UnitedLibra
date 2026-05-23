#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "database.h"
#include "databasemodels.h"
#include "dataline.h"
#include "messagebox.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTabWidget>

class Workspace : public QWidget
{
    Q_OBJECT

public:
    // Enum для типов данных
    enum DataType {
        Books = 1,      // Книги
        Authors = 2,    // Авторы
        Publishers = 3, // Издатели
        BookCopies = 4, // Копии книг
        Loans = 5,      // Выдачи
        Fines = 6,      // Штрафы
        Users = 7       // Пользователи
    };
    Q_ENUM(DataType)

    explicit Workspace(QWidget *parent = nullptr, DataType dataType = Books);

    // Геттер для типа данных
    DataType getDataType() const { return pageDataType; }
    QString getDataTypeName() const;

signals:
    // Сигналы об изменении данных и обновлении кнопок страниц
    void dataChanged();
    void updatePagesButtons();

private:
    // Методы ответственные за создание интерфейса и инициализации логики интерфейса
    void setupUI();
    void setupFilters();
    void setupMainButtons();
    void setupDataArea();
    void setupPagesButtons();

    // Создание линий данных
    void generateDataLines(int dataCount, int currentPage);
    // Заполнение линии данных
    void updateAvailableResults();
    void fillDataLines(int dataCount, int currentPage, QVector<QMap<QString, QVariant>> allData);
    // Обновление линий данных
    void updateData();
    // Открытие вкладки редактирования
    void openEditingTab(int dataId, const QMap<QString, QVariant> &dataMap, bool isNewData);

    // Метод для поиска QTabWidget
    QTabWidget* findParentTabWidget() const;

    QVBoxLayout *mainLayout; // Основное рабочее пространство
    QVBoxLayout *dataArea;  // Для хранения строк данных
    BookModel *db;
    QVector<QMap<QString, QVariant>> allData; // Загруженные из базы данных данные
    QLabel *pageInfo = nullptr;

    QString filterText = "";
    DataType pageDataType; // Тип данных

    int currentPage = 1; // Текущая страница
    int maxPages = 1; // Максимум страниц

    int currentResults = 0; // Доступные записи
    int maxResults = 0; // Всего записей

    bool doNotShowUpdateInfo = 1; // Показывать ли информацию о обновлении данных

    QVector<QPushButton*> pagesButtons; // Вектор кнопок перемещения по страницам
    QVector<DataLine*> allDataLines;
    QVector<int> selectedDataIds; // Вектор для хранения ID выбранных данных

private slots:
    // Слоты обрабатывающие взаимодействия с основными кнопками
    void onAddClicked();
    void onOpenClicked();
    void onDeleteClicked();
    void onUpdateClicked();

    // Слоты обрабатывающие взаимодействия с кнопками страниц
    void onPrevClicked();
    void onNextClicked();
    void onFirstClicked();
    void onLastClicked();
    void onPageClicked();

    // Слоты для фильтрации и поиска
    void onFilterTextChanged(const QString &text);
    void onSearchTextChanged(const QString &text);

    // Слот для обновления кнопок страниц
    void onUpdatePagesButtons();

    // Слот для добавления id данных в список выбранных
    void onDataLineToggled(bool checked, int dataId);
};

#endif