#include "webcamplayer.h"

WebcamPlayer::WebcamPlayer(QObject * parent)
    : QThread(parent) {
    stop();
}

/*
 * Open webcam device from index (0 for default webcam). Closes the already opened device
 */
bool WebcamPlayer::open(int device) {
    release();

    // Open new webcam && return result
    return capture.open(device);
}

/*
 * Start emitting frame data (via processedImage(QImage))
 */
void WebcamPlayer::play() {
    if (!isRunning()) {
        if (isStopped()) {
            stopped = false;
        }

        // Start running thread
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
            stop();
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
    }
}

bool WebcamPlayer::isStopped() const {
    return stopped;
}

/*
 * Release device from video capture, preventing interruption from other threads
 */
void WebcamPlayer::release() {
    mutex.lock();
    if (capture.isOpened() ) {
        stop();
        capture.release();
    }

    mutex.unlock();
}

/*
 * Stop emitting frame data
 */
void WebcamPlayer::stop() {
    stopped = true;
}

WebcamPlayer::~WebcamPlayer() {
    release();

    // Stop running thread
    wait();
}
