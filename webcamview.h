#ifndef WEBCAMVIEW_H
#define WEBCAMVIEW_H

#include <string>

#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QSettings>

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

    bool isClickToDrag = false;
    bool dragging = false;
    int mouseOriginX;
    int mouseOriginY;

    bool guidingLineEnabled = false;
    double guidingLinePos = 0.5;
    QColor guidingLineColor = Qt::black;

    // Copy of current image/frame
    QImage image;
    QImage snapshotImage;
    // Graphical representation of image in view
    QGraphicsPixmapItem imageItem;

protected slots:
    void handleError();
    void updateImage(QImage img);
    void setSnapshotImage(const QImage & img);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void leaveEvent(QEvent * event);
    void paintEvent(QPaintEvent * event);

    void setDragging(bool isDragging);
    bool isDragging();

public:

    Mode DEFAULT_MODE = PREVIEW;
    int DEFAULT_DEVICE = 0;

    WebcamView(QWidget * parent = nullptr);
    WebcamView(int device = 0, QWidget * parent = nullptr);
    ~WebcamView();

    void init(Mode mode, int device, QWidget * parent);
    bool openWebcam(int device);
    Mode getMode();
    void resize();
    void setMode(Mode mode);
    void setContrast(double contrast);
    void setClickToDragEnabled(bool isClickToDrag);
    bool isClickToDragEnabled();
    void setBrightness(double brightness);
    void setFilter(std::string filter);
    void setRotation(int angle);
    void setGuidingLineEnabled(bool guidingLineEnabled);
    void setGuidingLinePos(double percent);
    void setGuidingLineColor(QColor color);
    double getContrast();
    double getBrightness();
    std::string getFilter();
    int getWebcam();
    int getRotation();
    bool isGuidingLineEnabled();
    void processSnapshotImage();

signals:
    void modeChanged();

};

#endif // WEBCAMVIEW_H
