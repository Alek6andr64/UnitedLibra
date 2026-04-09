#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Workspace : public QWidget
{
    Q_OBJECT

public:
    explicit Workspace(QWidget *parent = nullptr);

private:
    void setupUI();
    void generateDataLines(int dataCount);

    QVBoxLayout *mainLayout; // Основное рабочее пространство
    QVBoxLayout *dataArea;  // Для хранения строк данных

private slots: // Слоты для обработки cобытий
    void onAddClicked();
    void onOpenClicked();
    void onDeleteClicked();
    void onUpdateClicked();
    void onPrevClicked();
    void onNextClicked();
    void onPageClicked();
    void onFilterTextChanged(const QString &text);
    void onSearchTextChanged(const QString &text);

signals:
    void dataChanged();  // Сигнал для оповещения об изменениях данных
};

#endif