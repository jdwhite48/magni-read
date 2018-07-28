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

        useMaxResolution();

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
            emit readError();
            break;
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

/*
 * Try common resolutions in 16:9 and 4:3 aspect ratios to find the best resolution that works with the webcam
 */
bool WebcamPlayer::useMaxResolution() {
    if (!capture.isOpened()) {
        return false;
    }

    double initWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double initHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    bool isResSet = false;

    // 8k UHD (4320p, 16:9)
    isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 7680) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 4320);
    if (!isResSet)
        // HUXGA (4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 6400) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 4800);
    if (!isResSet)
        // 5K UHD+ (16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 5120) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 2880);
    if (!isResSet)
        // HXGA (4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 4096) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 3072);
    if (!isResSet)
        // 4K UHD (2160p, 16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 3840) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 2160);
    if (!isResSet)
        // QUXGA (4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 3200) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 2400);
    if (!isResSet)
        // (1440p, 4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1440);
    if (!isResSet)
        // Full HD (1080p, 16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    if (!isResSet)
        // HD+ (16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 1600) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 900);
    if (!isResSet)
        // (1080p, 4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 1440) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    if (!isResSet)
        // HD (720p, 16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    if (!isResSet)
        // (720p, 4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 960) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    if (!isResSet)
        // qHD (16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 960) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 540);
    if (!isResSet)
        // SVGA (4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 800) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 600);
    if (!isResSet)
        // VGA (4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 640) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!isResSet)
        // nHD/Wide SD (360p, 16:9)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 640) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
    if (!isResSet)
        // (360p, 4:3)
        isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 480) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 360);

    // Set resolution back to previous resolution
    if (!isResSet) {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, initWidth);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, initHeight);
    }

    return isResSet;
}

WebcamPlayer::~WebcamPlayer() {
    release();

    // Stop running thread
    wait();
}
