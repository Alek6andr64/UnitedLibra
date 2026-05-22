#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "database.h"
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
    explicit Workspace(QWidget *parent = nullptr);

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
    void fillDataLines(int dataCount, int currentPage, QVector<QMap<QString, QVariant>> books);
    // Обновление линий данных
    void updateData();
    // Открытие вкладки редактирования
    void openEditingTab(int bookId, const QMap<QString, QVariant> &bookData, bool newBook);

    // Метод для поиска QTabWidget
    QTabWidget* findParentTabWidget() const;

    QVBoxLayout *mainLayout; // Основное рабочее пространство
    QVBoxLayout *dataArea;  // Для хранения строк данных
    Database db;
    QVector<QMap<QString, QVariant>> books; // Загруженные из базы данных книги
    QLabel *pageInfo = nullptr;

    QString filterText = "";

    int currentPage = 1; // Текущая страница
    int maxPages = 1; // Максимум страниц

    int currentResults = 0; // Доступные записи
    int maxResults = 0; // Всего записей

    bool doNotShowUpdateInfo = 1; // Показывать ли информацию о обновлении данных

    QVector<QPushButton*> pagesButtons; // Вектор кнопок перемещения по страницам
    QVector<DataLine*> allDataLines;
    QVector<int> selectedBookIds; // Вектор для хранения ID выбранных книг

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

    // Слот для добавления id книги в список выбранных
    void onDataLineToggled(bool checked, int bookId);
};

#endif