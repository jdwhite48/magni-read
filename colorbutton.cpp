#include "colorbutton.h"

ColorButton::ColorButton(const QColor & color, QWidget * parent)
    : QPushButton(parent) {
    this->setMinimumWidth(50);
    currentColor = color;
    connect(this, SIGNAL (clicked()), this, SLOT(chooseColor()));
}

QColor ColorButton::getColor() {
    return currentColor;
}

void ColorButton::setColor(const QColor & color) {
    currentColor = color;
    emit colorChanged(currentColor);
    this->update();
}

void ColorButton::chooseColor() {
    QColor color = QColorDialog::getColor(currentColor, this);
    if (color.isValid()) {
        setColor(color);
    }
}

/*
 * Draw a color swatch inside the button, padding the sides
 */
void ColorButton::paintEvent(QPaintEvent * event) {
    QPushButton::paintEvent(event);

    QRect swatch = event->rect();
    QPainter painter( this );
    // Make swatch less opaque if button is disabled
    if (!isEnabled()) {
        painter.setOpacity(0.5);
    }
    painter.setBrush( QBrush(currentColor) );
    painter.setPen(Qt::lightGray);
    swatch.adjust(colorPadding, colorPadding, -1-colorPadding, -1-colorPadding);
    painter.drawRect(swatch);

}
