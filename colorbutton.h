#ifndef COLORBUTTON_H
#define COLORBUTTON_H

// Derived class
#include <QPushButton>

// Implementation classes
#include <QColorDialog>
#include <QPainter>
#include <QPaintEvent>

class ColorButton : public QPushButton {
    Q_OBJECT

public:
    ColorButton(const QColor & color = Qt::black, QWidget * parent = nullptr);
    QColor getColor();

signals:
    void colorChanged(QColor);

public slots:
    void setColor(const QColor &);
    void chooseColor();
    void paintEvent(QPaintEvent * event);

private:
    QColor currentColor;
    int colorPadding = 15;

};

#endif // COLORBUTTON_H
