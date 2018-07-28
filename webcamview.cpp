#include "webcamview.h"

WebcamView::WebcamView(QWidget * parent)
    : QGraphicsView(parent)
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    int device = (settings.contains("webcam/deviceIndex"))
                  ? settings.value("webcam/deviceIndex").toInt()
                  : DEFAULT_DEVICE;

    init(DEFAULT_MODE, device, parent);
}

WebcamView::WebcamView(int device, QWidget * parent)
    : QGraphicsView(parent)
{
    init(DEFAULT_MODE, device, parent);
}

/*
 * Initialization of WebcamView
 */
void WebcamView::init(WebcamView::Mode mode, int device, QWidget * parent) {
    this->mode = mode;

    // Change viewport functionality & appearance
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    // Attach scene
    scene = new QGraphicsScene(parent);
    setScene(scene);

    // Setup video capture and load video
    videoPlayer = new WebcamPlayer(this);
    openWebcam(device);
    connect(videoPlayer, SIGNAL (processedImage(QImage)),
            this, SLOT (updateImage(QImage)));
    connect(videoPlayer, SIGNAL (readError()),
            this, SLOT (handleError()));

    // Initial display
    if (mode == SNAPSHOT) {
        // TODO: Instead of sample image, "choose webcam" image shown on screen
        updateImage(QImage(":/media/sampleImage.jpg"));
    }
    else if (mode == PREVIEW) {
        videoPlayer->play();
    }
    else if (mode == ERROR ) {
        // TODO: "Webcam not found" image shown on screen
        handleError();
    }

    // Prepare view for display
    this->show();
}

/*
 * Rescale image so that it keeps the aspect ratio, fills the entire viewport, and scrolls properly
 */
void WebcamView::updateImage(QImage img) {

    // Remove old image
    image = img;
    // Rescale new image so that it at least fills the viewport
    QPixmap pixmap = QPixmap::fromImage(img).scaled(
                static_cast<int>(this->size().width()),
                static_cast<int>(this->size().height()),
                Qt::KeepAspectRatioByExpanding, Qt::FastTransformation );
    imageItem.setPixmap(pixmap);

    if (scene->items().count() == 0) {
        scene->addItem(&imageItem);
    }

    // Prepare rescaled view for display
    this->show();
}

/*
 * Resize current image to fit the screen
 */
void WebcamView::resize() {
    if (!image.isNull()) {
        updateImage(image);
    }
}

/*
 * Change what is being viewed
 */
void WebcamView::setMode(WebcamView::Mode mode) {
    this->mode = mode;

    if (mode == PREVIEW) {
        videoPlayer->play();
    }
    else if (mode == SNAPSHOT) {
        videoPlayer->stop();
    }

    emit modeChanged();
}

WebcamView::Mode WebcamView::getMode() {
    return mode;
}

/*
 * Changes to error mode and stops video player if playing
 */
void WebcamView::handleError() {
    setMode(ERROR);
    videoPlayer->stop();
}

/*
 * Change the webcam to the index of the device specified
 */
bool WebcamView::openWebcam(int device) {
    bool isOpened = videoPlayer->open(device);
    if (!isOpened) {
        handleError();
    }
    else {
        switch (mode) {
            case PREVIEW:
                videoPlayer->play();
                break;
            case SNAPSHOT:
                break;
            case ERROR:
            default:
                handleError();
                break;
        }
    }

    return isOpened;
}
