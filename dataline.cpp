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
    lineLayout->setContentsMargins(0, 0, 0, 0);

    // Создаем чекбокс
    checkBox = new QCheckBox(lineWidget);
    connect(checkBox, &QCheckBox::toggled, this, &DataLine::onCheckBoxToggled);

    // Создаем надписи
    setupLabels();

    // Добавляем элементы в виджет
    lineLayout->addWidget(checkBox, 1);
    for (int i = 0; i < 8; i++) {
        if (i == 0 or i == 3 or i == 7) {
            lineLayout->addWidget(labels[i], 1);
        }
        else {
            lineLayout->addWidget(labels[i], 2);
        }
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
        label->setMinimumHeight(45);
        label->setStyleSheet("QLabel {color: #413F3F; font: bold;}");
        labels.append(label);
    }

    // Устанавливаем минимальные и максимальные ширины для разных колонок
    labels[0]->setMinimumWidth(40);
    labels[0]->setMaximumWidth(60);
    labels[1]->setMinimumWidth(170);

    //labels[1]->setMaximumWidth(240);
    labels[2]->setMinimumWidth(120);

    //labels[2]->setMaximumWidth(160);
    labels[3]->setMinimumWidth(50);
    labels[3]->setMaximumWidth(50);
    labels[4]->setMinimumWidth(200);

    //labels[4]->setMaximumWidth(360);
    labels[5]->setMinimumWidth(120);

    //labels[5]->setMaximumWidth(340);
    labels[6]->setMinimumWidth(180);

    //labels[6]->setMaximumWidth(340);
    labels[7]->setMinimumWidth(40);
    labels[7]->setMaximumWidth(60);
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
    setElidedText(bookData, "title", 1);
    setElidedText(bookData, "isbn", 2);
    setElidedText(bookData, "year", 3);
    setElidedText(bookData, "authors", 4);
    setElidedText(bookData, "publisher_name", 5);
    setElidedText(bookData, "categories", 6);
    setElidedText(bookData, "copy_count", 7);

    // Делаем чекбокс видимым
    checkBox->setVisible(true);
}


void DataLine::setElidedText(const QMap<QString, QVariant>& bookData, QString field, int index) {
    QFontMetrics metrics(labels[index]->font());
    QString text = metrics.elidedText(bookData[field].toString(), Qt::ElideRight, labels[index]->width());
    if (text.isEmpty()) {
        labels[index]->setText("Не указано");
    }
    labels[index]->setText(text);
    labels[index]->setToolTip(bookData[field].toString());
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
    checkBox->setVisible(false);
    setSelected(false);

}

void DataLine::onCheckBoxToggled(bool checked)
{
    // Устанавливаем состояние чекбокса и оповещаем подписчиков об этом
    setSelected(checked);
    emit toggled(checked, currentBookId);
}