#ifndef STYLESHEETLOADER_H
#define STYLESHEETLOADER_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QWidget>

class StyleSheetLoader
{
public:
    static QString loadStyleSheet(const QString &path)
    {
        QFile file(path);
        if (!file.open(QFile::ReadOnly)) {
            qDebug() << "Не удалось загрузить стиль:" << path;
            return "";
        }

        QString styleSheet = QLatin1String(file.readAll());
        file.close();

        return styleSheet;
    }

    static void applyStyleSheet(QWidget *widget, const QString &path)
    {
        QString styleSheet = loadStyleSheet(path);
        if (!styleSheet.isEmpty()) {
            widget->setStyleSheet(styleSheet);
        }
    }
};

#endif