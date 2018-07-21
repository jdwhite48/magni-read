#include "webcamplayer.h"

WebcamPlayer::WebcamPlayer(QObject * parent)
    : QThread(parent) {
    stopped = true;
}

/*
 * Closes any already opened camera, then opens video from file path and retreives FPS
 */
bool WebcamPlayer::open(std::string filename) {
    // Close any already opened camera
    mutex.lock();
    if (capture.isOpened()) {
        stopped = true;
        capture.release();
    }

    // Open new video
    capture.open(filename);
    mutex.unlock();

    if (capture.isOpened()) {
        frameRate = int(capture.get(CV_CAP_PROP_FPS));
        return true;
    }
    else return false;
}

/*
 * Load video from webcam device's index (0 for default)
 */
bool WebcamPlayer::open(int device) {
    // Close any already opened camera
    mutex.lock();
    if (capture.isOpened()) {


        stopped = true;
        capture.release();
    }

    // Open new webcam
    capture.open(device);
    mutex.unlock();

    if (capture.isOpened()) {
        // if 0, runs as fast as it can read
        frameRate = int(capture.get(CV_CAP_PROP_FPS));
        return true;
    }
    else return false;
}

/*
 * Toggle player and start running thread
 */
void WebcamPlayer::play() {
    if (!isRunning()) {
            if (isStopped()) {
                stopped = false;
            }

            // Start thread
            start(LowPriority);
    }
}

/*
 * Repeatedly capture frame data and convert to QImage
 */
void WebcamPlayer::run() {
    while (!stopped) {
        // Get next frame of video
        bool isRead = capture.read(frame);
        if (!isRead) {
            stopped = true;
        }
        // Switch from OpenCV's BGR to RGB format & convert to QImage
        if (frame.channels() == 3) {
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage( const_cast<unsigned char *>(RGBframe.data),
                RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);

        }
        else {
            img = QImage(const_cast<unsigned char *>(frame.data),
                frame.cols, frame.rows, QImage::Format_Indexed8);
        }

        emit processedImage(img);

        // Regulate frame rate interval if necessary
        if (frameRate > 0) {
            int delay = 1000 / frameRate;
            msleep( static_cast<unsigned long>(delay) );
        }
    }
}

bool WebcamPlayer::isStopped() const {
    return stopped;
}

void WebcamPlayer::stop() {
    stopped = true;
}

WebcamPlayer::~WebcamPlayer() {
    // Prevent termination of thread before VideoCapture is finished
    mutex.lock();

    stopped = true;
    capture.release();

    mutex.unlock();
    condition.wakeAll();

    // Stop running thread
    wait();
}
