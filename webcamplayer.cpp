#include "webcamplayer.h"

WebcamPlayer::WebcamPlayer(QObject * parent)
    : QThread(parent) {
    stop();
    brightness = 0;
    contrast = 1;
}

/*
 * Open webcam device from index (0 for default webcam). Closes the already opened device
 */
bool WebcamPlayer::open(int device) {
    release();

    curWebcam = device;

    // Open new webcam && return result
    int isOpened = capture.open(device);
    useMaxResolution();
    return isOpened;
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
        bool isRead = capture.read(rawFrame);
        if (!isRead) {
            stop();
            emit readError();
            break;
        }

        // Get raw image
        if (rawFrame.channels() == 3) {
            cv::cvtColor(rawFrame, rawRGBFrame, CV_BGR2RGB);
            rawImage = QImage( const_cast<unsigned char *>(rawRGBFrame.data),
                rawRGBFrame.cols, rawRGBFrame.rows, QImage::Format_RGB888);
        }
        else {
            rawImage = QImage(const_cast<unsigned char *>(rawFrame.data),
                rawFrame.cols, rawFrame.rows, QImage::Format_Indexed8);
        }\
        emit imageRead(rawImage);

        // Get processed image

        rawFrame.copyTo(frame);

        // image' = contrast * image + brightness
        frame.convertTo(frame, -1, contrast, brightness);

        // Rotate clockwise by the specified amount of degrees
        Point2f frameCenter(frame.cols/2.0F, frame.rows/2.0F);
        Mat rotMatrix = getRotationMatrix2D(frameCenter, -angle, 1.0);
        warpAffine(frame, frame, rotMatrix, frame.size());

        // Switch from OpenCV's BGR to RGB format & convert to QImage
        if (frame.channels() == 3) {
            cv::cvtColor(frame, RGBFrame, CV_BGR2RGB);

            if (filter == "Greyscale") {
                cv::cvtColor(RGBFrame, greyFrame, CV_RGB2GRAY);
                processedImage = QImage( const_cast<unsigned char *>(greyFrame.data),
                              greyFrame.cols, greyFrame.rows, QImage::Format_Grayscale8);
            }
            else if (filter == "Black and White") {
                cv::cvtColor(RGBFrame, greyFrame, CV_RGB2GRAY);
                cv::threshold(greyFrame, monoFrame, 100, 255, THRESH_BINARY );
                // Use Grayscale format, Mono forms scan lines
                processedImage = QImage( const_cast<unsigned char *>(monoFrame.data),
                              monoFrame.cols, monoFrame.rows, QImage::Format_Grayscale8);
            }
            else { // filter == "None"
                processedImage = QImage( const_cast<unsigned char *>(RGBFrame.data),
                    RGBFrame.cols, RGBFrame.rows, QImage::Format_RGB888);
            }
        }
        else {
            processedImage = QImage(const_cast<unsigned char *>(frame.data),
                frame.cols, frame.rows, QImage::Format_Indexed8);
        }

        emit imageProcessed(processedImage);
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
 * Set webcam to run with the best possible resolution (up to 8K UHD)
 */
bool WebcamPlayer::useMaxResolution() {
    if (!capture.isOpened()) {
        return false;
    }

    double initWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double initHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Set webcam resolution to hightest <= set value
    bool isResSet = capture.set(CV_CAP_PROP_FRAME_WIDTH, 7680) & capture.set(CV_CAP_PROP_FRAME_HEIGHT, 4320);

    // Set resolution back to previous resolution
    if (!isResSet) {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, initWidth);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, initHeight);
    }

    return isResSet;
}

/*
 * Set brightness (image delta) to a given value -256 < b < 256
 */
void WebcamPlayer::setBrightness(double b) {
    if (b > 255)
        brightness = 255;
    else if (b < -255)
        brightness = -255;
    else
        brightness = b;
}

/*
 * Set contrast (scaling factor of image) to a given value > 0
 */
void WebcamPlayer::setContrast(double a) {
    if (a <= 0) {
        contrast = 0.001;
    }
    else {
        contrast = a;
    }
}

/*
 * Set color filter to an identifiable filter
 */
void WebcamPlayer::setFilter(std::string filter) {
    if ( filter == "Black and White" || filter == "Greyscale") {
        this->filter = filter;
    }
    else {
        this->filter = "None";
    }
}

/*
 * Set angle of rotation for image
 */
void WebcamPlayer::setRotation(int angle) {
    this->angle = angle % 360;
}

double WebcamPlayer::getContrast() {
    return contrast;
}

double WebcamPlayer::getBrightness() {
    return brightness;
}

std::string WebcamPlayer::getFilter() {
    return filter;
}

int WebcamPlayer::getWebcam() {
    return curWebcam;
}

int WebcamPlayer::getRotation() {
    return angle;
}

WebcamPlayer::~WebcamPlayer() {
    release();

    // Stop running thread
    wait();
}
