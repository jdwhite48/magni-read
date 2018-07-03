#include "webcamview.h"

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

    // Add image to scene
    image = QPixmap("images//sampleImage.jpg");

    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);

    // Prepare view for display
    this->show();
}

/*
 * Rescale image so that it keeps the aspect ratio, fills the entire viewport, and scrolls properly
 */
void WebcamView::rescaleImage() {

    // Remove old image
    scene->removeItem(imageItem);
    delete imageItem;

    // Rescale new image so that it at least fills the viewport
    image = QPixmap("images//sampleImage.jpg").scaled(
                static_cast<int>(this->size().width()),
                static_cast<int>(this->size().height()),
                Qt::KeepAspectRatioByExpanding,
                Qt::FastTransformation );
    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);

    // Make scene larger scene than viewport if necessary (allows scrolling of zoomed image)
    scene->setSceneRect(0, 0, image.width(), image.height());

    // Prepare rescaled view for display
    this->show();
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
