#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QLabel>
#include <QKeyEvent>
#include <QSet>
#include <QMap>
#include <QStringListModel>
#include <QPushButton>
#include <QSqlQuery>

class Tag : public QWidget
{
    Q_OBJECT
public:
    explicit Tag(int id, const QString& name, QWidget* parent = nullptr);
    int id() const { return dataId; }

signals:
    // Сигнал для удаления тега
    void removeRequested(int id);

protected:
    // Слот для обработки нажатия на кнопку закрытия
    void onCloseClicked();

private:
    int dataId;                // ID тега
    QWidget* tagWidget;        // Виджет-контейнер для тега
    QLabel* dataLabel;         // Метка с именем тега
    QPushButton* closeBtn;     // Кнопка закрытия
};

class TagEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TagEditor(QWidget* parent = nullptr, const QString& initialText = "Нажмите Enter или Пробел...");
    void setupData(QSqlQuery& data);
    void setupTags(QString& tags);
    QVector<int> selectedTags;              // ID выбранных тегов

protected:
    // Фильтр событий для обработки нажатий клавиш в поле ввода
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    // Слот для обработки выбора варианта автодополнения
    void onCompletion(const QString& text);
    // Слот для удаления тега
    void removeTag(int id);

private:
    // Методы ответственные за инициализацию интерфейса и логики
    void setupAutocompletion();
    void addTagOnLine(int id, const QString& name);

    QHBoxLayout* tagEditorLayout;           // Горизонтальный layout для тегов и поля ввода
    QLineEdit* lineEdit;                    // Поле ввода текста
    QCompleter* autocompletion;             // Комплектор для автодополнения
    QStringListModel* autocompletionModel;  // Модель данных для автодополнения
    QMap<QString, int> allTags;             // Все теги из БД
};

#endif