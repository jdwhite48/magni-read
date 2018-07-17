#ifndef WEBCAMPLAYER_H
#define WEBCAMPLAYER_H

#include <QThread>

#include <QImage>
#include <QMutex>
#include <QWaitCondition>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string>

using namespace cv;

class WebcamPlayer : public QThread
{
    Q_OBJECT

private:
    bool stopped;
    QMutex mutex;
    QWaitCondition condition;
    Mat RGBframe;
    Mat frame;
    int frameRate;
    VideoCapture capture;
    QImage img;

signals:
    void processedImage(const QImage & image);

protected:
    void run();

public:

    WebcamPlayer(QObject * parent = nullptr);
    ~WebcamPlayer();

    bool open(std::string filename);
    bool open(int device = 0);
    void play();
    void stop();
    bool isStopped() const;
};

#endif // WEBCAMPLAYER_H
