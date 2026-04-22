#ifndef DATALINE_H
#define DATALINE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

class DataLine : public QWidget
{
    Q_OBJECT

public:
    explicit DataLine(QWidget *parent = nullptr);

    void setData(const QMap<QString, QVariant>& bookData, int bookId);
    void setSelected(bool selected);
    void setVisible(bool visible);
    void reset();

    QCheckBox* getCheckBox() const { return checkBox; }
    int getBookId() const { return currentBookId; }
    bool isSelected() const { return checkBox->isChecked(); }
    void setChecked(bool checked) { checkBox->setChecked(checked); }

signals:
    void toggled(bool checked, int bookId);

private slots:
    void onCheckBoxToggled(bool checked);

private:
    void setupLabels();
    void setupStyle();

    QHBoxLayout *mainLayout;
    QWidget *lineWidget;      // Виджет-контейнер для строки
    QHBoxLayout *lineLayout;  // Layout для lineWidget
    QCheckBox *checkBox;
    QVector<QLabel*> labels;

    int currentBookId;
};

#endif