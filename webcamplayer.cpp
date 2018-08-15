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
        bool isRead = capture.read(frame);
        if (!isRead) {
            stop();
            emit readError();
            break;
        }

        // Save unmodified image
        rawImage = convertMatToQImage(frame);
        emit imageRead(rawImage);

        // Process frame and save modified image
        frame = processImage(frame);
        processedImage = convertMatToQImage(frame);
        emit imageProcessed(processedImage);
    }
}

/*
 * Change contrast, brightness, and rotation of image. Convert colors depending on filter string
 */
Mat WebcamPlayer::processImage(Mat cvImg) {
    // image' = contrast * image + brightness
    cvImg.convertTo(cvImg, -1, contrast, brightness);

    // Rotate clockwise by the specified amount of degrees
    Point2f frameCenter(cvImg.cols/2.0F, cvImg.rows/2.0F);
    Mat rotMatrix = getRotationMatrix2D(frameCenter, -angle, 1.0);
    // Determine bounding rectangle
    Rect2f boundsBox = cv::RotatedRect(cv::Point2f(), cvImg.size(), -angle).boundingRect2f();
    // Adjust transformation matrix to fit full image
    rotMatrix.at<double>(0,2) += boundsBox.width/2.0 - cvImg.cols/2.0;
    rotMatrix.at<double>(1,2) += boundsBox.height/2.0 - cvImg.rows/2.0;

    warpAffine(cvImg, cvImg, rotMatrix, cvImg.size(), INTER_NEAREST);

    if (cvImg.channels() == 3) {
        if (filter == "Greyscale" || filter == "Grayscale") {
            Mat greyFrame;

            // Convert BGR to single color channel (CV_8UC3 -> CV_8UC1)
            cv::cvtColor(cvImg, greyFrame, CV_BGR2GRAY);

            return greyFrame;
        }
        else if (filter == "Black and White") {
            Mat monoFrame;

            // Convert BGR to single color channel (CV_8UC3 -> CV_8UC1)
            cv::cvtColor(cvImg, monoFrame, CV_BGR2GRAY);
            // Make any pixel >= 100 white, else black (changing brightness/contrast affects this threshhold)
            cv::threshold(monoFrame, monoFrame, 100, 255, THRESH_BINARY );

            return monoFrame;
        }
        else { // filter == "None"

            return cvImg;
        }
    }
    else {

        return cvImg;
    }

}

/*
 * Creates a Mat that is a deep copy of the QImage
 */
Mat WebcamPlayer::convertQImageToMat(QImage QImg) {
    Mat cvImg;
    switch (QImg.format()) {
        case QImage::Format_RGB888 :
            cvImg = cv::Mat(QImg.height(), QImg.width(),
                              CV_8UC3, const_cast<uchar*>(QImg.bits()), uint(QImg.bytesPerLine())).clone();
            break;
        case QImage::Format_Grayscale8 :
        case QImage::Format_Indexed8 : {
            cvImg = cv::Mat(QImg.height(), QImg.width(),
                              CV_8UC1, const_cast<uchar*>(QImg.bits()), uint(QImg.bytesPerLine())).clone();
            break;
        }
        default:
            break;
    }

    // Switch from Qt's RGB format to OpenCV's BGR format
    if (frame.channels() == 3) {
        Mat cvBGRImg;
        cv::cvtColor(cvImg, cvBGRImg, CV_RGB2BGR);
        return cvBGRImg;
    }
    else {
        return cvImg;
    }
}

QImage WebcamPlayer::convertMatToQImage(Mat cvImg) {
    QImage QImg;
    if (cvImg.channels() == 3) {

        // Switch from OpenCV's BGR format to Qt's RGB format
        Mat RGBImg;
        cv::cvtColor(cvImg, RGBImg, CV_BGR2RGB);

        switch (RGBImg.type()) {
            case CV_8UC3 :
                QImg = QImage( const_cast<unsigned char *>(RGBImg.data),
                    RGBImg.cols, RGBImg.rows, QImage::Format_RGB888).copy();
                break;
            case CV_8UC1 : // or CV_8U
                QImg = QImage( const_cast<unsigned char *>(RGBImg.data),
                    RGBImg.cols, RGBImg.rows, QImage::Format_Grayscale8).copy();
                break;

        }
    }
    else {
        QImg = QImage( const_cast<unsigned char *>(cvImg.data),
            cvImg.cols, cvImg.rows, QImage::Format_Indexed8).copy();
    }

    return QImg;
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
