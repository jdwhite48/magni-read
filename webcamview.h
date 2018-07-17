#ifndef WEBCAMVIEW_H
#define WEBCAMVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

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

    // For snapshot mode
    QPixmap image;
    QGraphicsPixmapItem * imageItem = nullptr;

    // For preview mode
    QGraphicsPixmapItem * frameItem = nullptr;

protected:
    void setMode(Mode mode);

protected slots:
    void updatePlayer(QImage img);

public:

    Mode DEFAULT_MODE = PREVIEW;

    WebcamView(QWidget * parent = nullptr);
    WebcamView(Mode mode, QWidget * parent = nullptr);

    void init(Mode mode, QWidget * parent);
    Mode getMode();
    void playVideo();
    void rescaleImage();
    void stopVideo();

};

#endif // WEBCAMVIEW_H
