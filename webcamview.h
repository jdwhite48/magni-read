#ifndef WEBCAMVIEW_H
#define WEBCAMVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

class WebcamView : public QGraphicsView {

    Q_OBJECT
public:
    enum Mode : int {
        // Display video feed from webcam
        PREVIEW = 0,
        // Display still image from webcam when the mode changed
        SNAPSHOT = 1,
        // Webcam not detected
        ERROR = 2,
    };

private:
    WebcamView::Mode mode;


    QGraphicsScene * scene = nullptr;

    // For snapshot mode
    QPixmap image;
    QGraphicsPixmapItem * imageItem = nullptr;

    // For preview mode

public:

    Mode DEFAULT_MODE = SNAPSHOT;

    WebcamView(QWidget * parent = nullptr);
    WebcamView(Mode mode, QWidget * parent = nullptr);

    void init(Mode mode, QWidget * parent);
    Mode getMode();
    void rescaleImage();
    void setMode(Mode mode);

};

#endif // WEBCAMVIEW_H
