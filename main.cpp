#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Use large font (18 point size, system default if larger)
    QFont defaultFont = a.font();
    QFont _font(defaultFont.family(),
                (defaultFont.pointSize() > 18) ? defaultFont.pointSize() : 18);
    a.setFont(_font);

    MainWindow w;
    w.show();

    return a.exec();
}
