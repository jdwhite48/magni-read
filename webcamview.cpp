#include "webcamview.h"

#include <iostream>
#include <opencv2/cvconfig.h>

WebcamView::WebcamView(QWidget * parent)
    : QGraphicsView(parent)
{
    init(DEFAULT_MODE, parent);
}

WebcamView::WebcamView(WebcamView::Mode mode, QWidget * parent)
    : QGraphicsView(parent)
{
    init(mode, parent);
}

/*
 * Initialization of WebcamView
 */
void WebcamView::init(WebcamView::Mode mode, QWidget * parent) {
    this->mode = mode;

    // Change viewport functionality & appearance
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Attach scene
    scene = new QGraphicsScene(parent);
    setScene(scene);

    // Setup video capture and load video
    videoPlayer = new WebcamPlayer(this);
    videoPlayer->open("media//vtest.avi");
    connect(videoPlayer, SIGNAL (processedImage(QImage)),
            this, SLOT (updateImage(QImage)));

    if (mode == SNAPSHOT) {
        // TODO: Instead of sample image, "choose webcam" image shown on screen
        updateImage(QImage("media//sampleImage.jpg"));
    }
    else if (mode == PREVIEW) {
        // Play video stream
        videoPlayer->play();
    }
    else if (mode == ERROR ) {
        // TODO: Error image shown on screen
    }

    // Prepare view for display
    this->show();
}

/*
 * Rescale image so that it keeps the aspect ratio, fills the entire viewport, and scrolls properly
 */
void WebcamView::updateImage(QImage img) {

    // Remove old image
    scene->removeItem(imageItem);
    delete imageItem;

    image = img;
    // Rescale new image so that it at least fills the viewport
    imageItem = new QGraphicsPixmapItem(QPixmap::fromImage(img).scaled(
                static_cast<int>(this->size().width()),
                static_cast<int>(this->size().height()),
                Qt::KeepAspectRatioByExpanding, Qt::FastTransformation ));

    // Make scene larger scene than viewport if necessary (allows scrolling of zoomed image)
    scene->setSceneRect(0, 0, img.width(), img.height());

    scene->addItem(imageItem);

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
}

WebcamView::Mode WebcamView::getMode() {
    return mode;
}

/*
 * Changes to preview mode and plays video player
 */
void WebcamView::playVideo() {
    setMode(PREVIEW);
    videoPlayer->play();
}

/*
 * Changes to snapshot mode and stops video player
 */
void WebcamView::stopVideo() {
    setMode(SNAPSHOT);
    videoPlayer->stop();
}
