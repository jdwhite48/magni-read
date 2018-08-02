#include "webcamview.h"

WebcamView::WebcamView(QWidget * parent)
    : QGraphicsView(parent)
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    int device = (settings.contains("webcam/deviceIndex"))
                  ? settings.value("webcam/deviceIndex").toInt()
                  : DEFAULT_DEVICE;

    if (settings.contains("controls/clickToDrag")) {
        setClickToDragEnabled( settings.value("controls/clickToDrag").toBool() );
    }

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
    setCursor(Qt::OpenHandCursor);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    this->setMouseTracking(true);

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

void WebcamView::setBrightness(double brightness) {
    videoPlayer->setBrightness(brightness);
}

double WebcamView::getBrightness() {
    return videoPlayer->getBrightness();
}

void WebcamView::setContrast(double contrast) {
    videoPlayer->setContrast(contrast);
}

double WebcamView::getContrast() {
    return videoPlayer->getContrast();
}

void WebcamView::setFilter(std::string filter) {
    videoPlayer->setFilter(filter);
}

std::string WebcamView::getFilter() {
    return videoPlayer->getFilter();
}

int WebcamView::getWebcam() {
    return videoPlayer->getWebcam();
}

void WebcamView::setRotation(int angle) {
    videoPlayer->setRotation(angle);
}

int WebcamView::getRotation() {
    return videoPlayer->getRotation();
}

/*
 * Change whether they want click to start and stop dragging, or hold to drag and drop
 */
void WebcamView::setClickToDragEnabled(bool isClickToDrag) {
    this->isClickToDrag = isClickToDrag;
    if (isClickToDrag) {
        // Use custom drag functions (click to start dragging, move mouse to move image, click to stop dragging)
        setDragMode(QGraphicsView::NoDrag);
    }
    else {
        // Hold mouse button down to drag and release to drop instead
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
}

bool WebcamView::isClickToDragEnabled() {
    return isClickToDrag;
}

/*
 * If currently dragging using clickToDrag
 */
void WebcamView::setDragging(bool isDragging) {
    this->dragging = isDragging;

}

bool WebcamView::isDragging() {
    return dragging;
}

/*
 * Changes to error mode and stops video player if playing
 */
void WebcamView::handleError() {
    setMode(ERROR);
    videoPlayer->stop();
}

/*
 * Left click to start dragging just by moving the mouse. Left click again to stop
 */
void WebcamView::mousePressEvent(QMouseEvent * event) {
    QGraphicsView::mousePressEvent(event);

    if (!isClickToDrag) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        mouseOriginX = event->x();
        mouseOriginY = event->y();

        // Turn off dragging to prevent resetting cursor

        dragging = !dragging;
        if (isDragging()) {
            setCursor(Qt::ClosedHandCursor);
        }
        else {
            setCursor(Qt::OpenHandCursor);
        }
    }
}

/*
 * Move image with the mouse if dragging
 */
void WebcamView::mouseMoveEvent(QMouseEvent * event) {
    QGraphicsView::mouseMoveEvent(event);

    if (!isClickToDrag) {
        return;
    }

    // Move image to center on where the mouse is on the image
    if (isDragging()) {
        QPointF oldPt = mapToScene(mouseOriginX, mouseOriginY);
        QPointF newPt = mapToScene(event->pos());
        QPointF delta = newPt - oldPt;

        // Translate relative to mouse
        ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::NoAnchor);
        translate(delta.x(), delta.y());

        // Reset transformation anchor
        setTransformationAnchor(anchor);

        mouseOriginX = event->x();
        mouseOriginY = event->y();
    }
}

/*
 * Stop dragging image if mouse leaves view
 */
void WebcamView::leaveEvent(QEvent * event) {
    QGraphicsView::leaveEvent(event);

    if (isClickToDrag) {
        setDragging(false);
        setCursor(Qt::OpenHandCursor);
    }
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
