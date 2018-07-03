#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Use large font (18 point size, system default if larger)
    QFont defaultFont = a.font();
    QFont _font(defaultFont.family(),
                (defaultFont.pointSize() > 18) ? defaultFont.pointSize() : 18); 
    a.setFont(_font);

    // Use custom stylesheet
    QFile file("stylesheet.css");
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet(file.readAll());
        // Replace whitespace characters with normal space
        stylesheet = stylesheet.simplified();
        a.setStyleSheet(stylesheet);
    }
    file.close();
    MainWindow w;
    w.show();

    return a.exec();
}
