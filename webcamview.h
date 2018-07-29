#ifndef WEBCAMVIEW_H
#define WEBCAMVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSettings>
#include <QWheelEvent>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>

#include "webcamplayer.h"

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

    WebcamPlayer * videoPlayer;
    QGraphicsScene * scene = nullptr;

    // Copy of current image/frame
    QImage image;
    // Graphical representation of image in view
    QGraphicsPixmapItem imageItem;

protected slots:
    void handleError();
    void updateImage(QImage img);

public:

    Mode DEFAULT_MODE = PREVIEW;
    int DEFAULT_DEVICE = 0;

    WebcamView(QWidget * parent = nullptr);
    WebcamView(int device = 0, QWidget * parent = nullptr);

    void init(Mode mode, int device, QWidget * parent);
    bool openWebcam(int device);
    Mode getMode();
    void resize();
    void setMode(Mode mode);
    void setContrast(double contrast);
    void setBrightness(double brightness);
    double getContrast();
    double getBrightness();

signals:
    void modeChanged();

};

#endif // WEBCAMVIEW_H
