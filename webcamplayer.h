#ifndef WEBCAMPLAYER_H
#define WEBCAMPLAYER_H

// Parent class
#include <QThread>

// Implementation classes
#include <QImage>
#include <QMutex>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

/*
 * Captures video from a webcam and sends frame data as a QImage
 */
class WebcamPlayer : public QThread {
    Q_OBJECT

private:
    bool stopped;
    QMutex mutex;
    Mat frame;
    Mat RGBframe;
    VideoCapture capture;
    QImage img;

    double contrast; // "Alpha" value as scaling factor (multiplication)
    double brightness; // "Beta" value as image delta (addition)

protected:
    void run();

public:
    WebcamPlayer(QObject * parent = nullptr);
    ~WebcamPlayer();

    bool open(int device = 0);
    void release();
    void play();
    void stop();
    bool isStopped() const;
    bool useMaxResolution();
    void setBrightness(double b);
    void setContrast(double a);
    double getBrightness();
    double getContrast();

signals:
    void processedImage(const QImage & image);
    void readError();
};

#endif // WEBCAMPLAYER_H
