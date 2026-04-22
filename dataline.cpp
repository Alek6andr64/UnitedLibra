#include "dataline.h"

DataLine::DataLine(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);

    // Убираем отступы между линиями данных
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Создаем виджет для строки
    lineWidget = new QWidget(this);
    lineLayout = new QHBoxLayout(lineWidget);

    // Уменьшаем отступы между контентом
    lineLayout->setContentsMargins(5, 2, 5, 2);

    // Создаем чекбокс
    checkBox = new QCheckBox(lineWidget);
    connect(checkBox, &QCheckBox::toggled, this, &DataLine::onCheckBoxToggled);

    // Создаем надписи
    setupLabels();

    // Добавляем элементы в виджет
    lineLayout->addWidget(checkBox, 1);
    for(QLabel* label : labels) {
        lineLayout->addWidget(label, 2);
    }
    lineLayout->addStretch(1);

    // Добавляем виджет в отображение
    mainLayout->addWidget(lineWidget);

    // Задаем стиль линий
    setupStyle();
}

void DataLine::setupLabels()
{
    // Создаем 8 строк данных
    for (int i = 0; i < 8; i++) {
        QLabel* label = new QLabel(lineWidget);
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        label->setFixedHeight(45);
        label->setStyleSheet("QLabel {padding: 5px; color: #413F3F; font: bold;}");
        labels.append(label);
    }

    // Устанавливаем минимальные ширины для разных колонок
    labels[0]->setMinimumWidth(50);
    labels[1]->setMinimumWidth(200);
    labels[2]->setMinimumWidth(150);
    labels[3]->setMinimumWidth(50);
    labels[4]->setMinimumWidth(100);
    labels[5]->setMinimumWidth(100);
    labels[6]->setMinimumWidth(50);
    labels[7]->setMinimumWidth(50);
}

void DataLine::setupStyle()
{
    // Задание стиля и размеров чекбокса
    checkBox->setMinimumWidth(30);
    checkBox->setFixedHeight(45);
    checkBox->setStyleSheet(
        "QCheckBox {padding: 10px; border-radius: 5px;}"
        "QCheckBox::indicator { background-color: #FFFFFF; border-radius: 5px; }"
        "QCheckBox::indicator:checked { background-color: #413F3F; border-radius: 5px;}"
        );

}

void DataLine::setData(const QMap<QString, QVariant>& bookData, int bookId)
{
    // Синхронизируем id книги с id строки
    currentBookId = bookId;

    // Задаем данные
    labels[0]->setText(QString::number(bookId));
    labels[1]->setText(bookData["title"].toString());
    labels[2]->setText(bookData["isbn"].toString());
    labels[3]->setText(QString::number(bookData["year"].toInt()));
    labels[4]->setText(bookData["author_name"].toString());
    labels[5]->setText(QString::number(bookData["publisher_id"].toInt()));
    labels[6]->setText(bookData["genre_name"].toString());
    labels[7]->setText(QString::number(bookData["copy_count"].toInt()));

    // Делаем линию видимой
    setVisible(true);
}

void DataLine::setSelected(bool selected)
{
    // Меняем цвет фона линии
    QString style = selected ?"background-color: #EAEAEA;" : "background-color: #F6F6F6;";
    lineWidget->setStyleSheet(style);
}

void DataLine::setVisible(bool visible)
{
    // Делаем линию видимой, убираем выделение чекбокса если его не видно
    QWidget::setVisible(visible);
    if (!visible) {
        checkBox->setChecked(false);
    }
}

void DataLine::reset()
{
    // Скрываем линию данных
    currentBookId = -1;
    for (QLabel* label : labels) {
        label->clear();
    }
    setVisible(false);
    setSelected(false);
}

void DataLine::onCheckBoxToggled(bool checked)
{
    // Устанавливаем состояние чекбокса и оповещаем подписчиков об этом
    setSelected(checked);
    emit toggled(checked, currentBookId);
}