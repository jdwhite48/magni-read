#ifndef WEBCAMVIEW_H
#define WEBCAMVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>


class WebcamView : public QGraphicsView {

    Q_OBJECT

private:
    QGraphicsScene * scene;
    QPixmap image;
    QGraphicsPixmapItem * imageItem;

public:
    WebcamView();
    WebcamView(QWidget * parent);
    void initGraphics();
    void rescaleImage();

};

#endif // WEBCAMVIEW_H
