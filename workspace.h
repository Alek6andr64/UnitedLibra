#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

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

    QVector<QPair<QCheckBox*, QVector<QLabel*>>> generateDataLines(int dataCount, int currentPage); // Создание линий данных
    QVector<QPair<QCheckBox*, QVector<QLabel*>>> fillDataLines(int dataCount, int currentPage,
            QVector<QPair<QCheckBox*, QVector<QLabel*>>> allDataLines, QVector<QMap<QString, QVariant>> books); // Заполнение линии данных
    void updateAvailableResults();

    QVBoxLayout *mainLayout; // Основное рабочее пространство
    QVBoxLayout *dataArea;  // Для хранения строк данных
    QVector<QMap<QString, QVariant>> books; // Загруженные из базы данных книги
    QLabel *pageInfo = nullptr;

    int currentPage = 1; // Текущая страница
    int maxPages = 1; // Максимум страниц

    int currentResults = 0; // Доступные записи
    int maxResults = 0; // Всего записей

    QVector<QPushButton*> pagesButtons; // Вектор кнопок перемещения по страницам
    QVector<QPair<QCheckBox*, QVector<QLabel*>>> allDataLines; // Вектор линий данных

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
    void onFilterTextChanged(const QString &text);
    void onSearchTextChanged(const QString &text);
    void onUpdatePagesButtons();
};

#endif