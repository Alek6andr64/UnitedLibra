#include "tageditor.h"
#include <QVBoxLayout>
#include <QDebug>

Tag::Tag(int id, const QString& name, QWidget* parent): QWidget(parent), dataId(id) {
    // Создаем виджет для тега
    tagWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(tagWidget);

    QHBoxLayout *layout = new QHBoxLayout(tagWidget);

    // Устанавливаем отступы контента
    layout->setContentsMargins(8, 4, 8, 4);
    layout->setSpacing(4);

    // Текстовое поле с текстом тега
    dataLabel = new QLabel(name, tagWidget);
    layout->addWidget(dataLabel);

    // Добавляем крестик для убирания тега и подключаем функционал
    closeBtn = new QPushButton("⨯", tagWidget);
    connect(closeBtn, &QPushButton::clicked, this, &Tag::onCloseClicked);
    layout->addWidget(closeBtn);

    // Применяем стили к контейнеру
    setupStyle();
}

void Tag::onCloseClicked() {
    // Удаляем кнопку, а потом и тег
    emit removeRequested(dataId);
    deleteLater();
}

void Tag::setupStyle() {
    closeBtn->setStyleSheet("QPushButton { background: transparent; color: white; font-weight: bold; border: none; }"
                            "QPushButton:hover { color: #413F3F; }");
    QSize size = closeBtn->sizeHint();
    closeBtn->setMaximumSize(size);

    // Применяем стили к контейнеру
    tagWidget->setStyleSheet(R"(
        QWidget {
            border: 1px solid #4a90e2;
            border-radius: 6px;
            background: #4a90e2;
        }
        QWidget:hover {
            background: #357abd;
            border-color: #357abd;
        }
    )");

    // Делаем текст белым
    dataLabel->setStyleSheet("QLabel { color: white; }");

    // Курсор-рука для всего контейнера
    tagWidget->setCursor(Qt::PointingHandCursor);
}

TagEditor::TagEditor(QWidget* parent, const QString& initialText): QWidget(parent) {
    // Область редактирования текста
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Контейнер для стилизации границы и фона
    QWidget* container = new QWidget(this);
    container->setObjectName("authorsContainer");

    // Область для размещения тегов и поля ввода
    tagEditorLayout = new QHBoxLayout(container);

    // Устанавливаем отступы контента
    tagEditorLayout->setSpacing(6);
    tagEditorLayout->setContentsMargins(6, 4, 6, 4);

    // Поле ввода текста
    lineEdit = new QLineEdit(container);
    lineEdit->setPlaceholderText(initialText);

    // Устанавливаем фильтр событий (для обработки клавиш)
    lineEdit->installEventFilter(this);

    // Устанавливаем подсказки к тексту
    setupAutocompletion();

    // Обработка ввода текста
    connect(lineEdit, &QLineEdit::textEdited, this, &TagEditor::onCompletion);

    // Добавляем поле ввода
    tagEditorLayout->addWidget(lineEdit);
    mainLayout->addWidget(container);

    // Применяем стили
    setupStyle();
}

void TagEditor::setupAutocompletion() {
    // Модель данных для списка авторов
    autocompletionModel = new QStringListModel(this);
    autocompletion = new QCompleter(autocompletionModel, this);

    // Регистронезависимый поиск в любой части слова
    autocompletion->setCaseSensitivity(Qt::CaseInsensitive);
    autocompletion->setFilterMode(Qt::MatchContains);

    // Устанавливаем автодополнение
    lineEdit->setCompleter(autocompletion);

    // Установка варианта из автодополнения
    connect(autocompletion, QOverload<const QString &>::of(&QCompleter::activated), this, &TagEditor::onCompletion);
}

void TagEditor::setupData(QSqlQuery& data) {
    // Загружаем данные
    while (data.next()) {
        allTags[data.value(1).toString()] = data.value(0).toInt();
    }

    // Устанавливаем список имен в модель для автодополнения
    autocompletionModel->setStringList(allTags.keys());
}


void TagEditor::onCompletion(const QString& text) {
    if (!text.isEmpty() && allTags.contains(text)) {
        addTagOnLine(allTags[text], text);
        lineEdit->clear();
    }
}

void TagEditor::addTagOnLine(int id, const QString& name) {
    // Проверяем был ли добавлен автор ранее
    if (selectedTags.contains(id))
        return;

    // Сохраняем автора
    selectedTags.append(id);

    // Создаем визуальный тег, вставляем перед полем ввода
    auto tag = new Tag(id, name, this);
    tagEditorLayout->insertWidget(tagEditorLayout->count() - 1, tag);

    // Подключаем сигнал удаления тега
    connect(tag, &Tag::removeRequested, this, &TagEditor::removeTag);
    lineEdit->clear();
}

void TagEditor::removeTag(int id) {
    // Проверяем, существует ли автор с таким ID
    if (!selectedTags.contains(id))
        return;

    // Удаляем из структуры данных
    selectedTags.removeAll(id);

    // Ищем соответствующий тег
    for (int i = 0; i < tagEditorLayout->count(); ++i) {
        QLayoutItem* item = tagEditorLayout->itemAt(i);
        if (item && item->widget()) {

            // Проверяем что это тег и что id совпадают
            Tag* tag = qobject_cast<Tag*>(item->widget());
            if (tag && tag->id() == id) {
                // Убираем из layout
                tagEditorLayout->removeWidget(tag);
                tag->deleteLater();
                break;
            }
        }
    }

    // Возвращаем фокус в поле ввода
    lineEdit->setFocus();
}

bool TagEditor::eventFilter(QObject* obj, QEvent* event) {
    if (obj == lineEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        // Delete при пустом поле ввода - удаляем последний тег
        if (key->key() == Qt::Key_Backspace && lineEdit->text().isEmpty()) {
            if (!selectedTags.isEmpty()) {
                int lastId = selectedTags.last();
                removeTag(lastId);

                // Событие обработано
                return true;
            }
        }

        // Enter - добавляем автора из текущего текста
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter) {
            QString text = lineEdit->text().trimmed();
            if (!text.isEmpty() && allTags.contains(text)) {
                addTagOnLine(allTags[text], text);
                lineEdit->clear();

                // Событие обработано
                return true;
            }
        }
    }

    return false;
}

void TagEditor::setupStyle() {
    // Стили для контейнера
    findChild<QWidget*>("authorsContainer")->setStyleSheet(R"(
        #authorsContainer {
            border: 1px solid #ccc;
            border-radius: 4px;
            background: white;
        }
        #authorsContainer:focus-within {
            border-color: #66afe9;
            outline: none;
        }
    )");

    // Стили для поля ввода
    lineEdit->setStyleSheet(R"(
        QLineEdit {
            border: none;
            padding: 4px;
            background: transparent;
        }
        QLineEdit:focus {
            outline: none;
        }
    )");
}