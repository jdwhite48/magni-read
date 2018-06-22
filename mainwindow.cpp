#include "mainwindow.h"

#include <iostream>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window layout
    window = new QWidget(this);
    QGridLayout * wLayout = createMainLayout();

    window->setLayout(wLayout);
    this->setCentralWidget(window);

    // Set window title
    this->setWindowTitle(tr("MagniRead"));

    // Maximize window
    this->setWindowState(Qt::WindowMaximized);
}

/*
 * Layout for displaying basic image modification buttons
 */
QVBoxLayout * MainWindow::createButtonLayout() {
    buttonLayout = new QVBoxLayout(this);

    QLabel * buttonLabel = new QLabel("Buttons:", this);
    QPushButton * okButton = new QPushButton("OK", this);

    buttonLayout ->addWidget(buttonLabel);
    buttonLayout->addWidget(okButton);

    return buttonLayout;
}

/*
 * Layout for displaying graphics and button for advanced image modifications
 */
QVBoxLayout * MainWindow::createGraphicsLayout() {
    graphicsLayout = new QVBoxLayout(this);

    initGraphics();

    QPushButton * settingsButton = new QPushButton("Settings", this);

    graphicsLayout->addWidget(view);
    graphicsLayout->addWidget(settingsButton);

    return graphicsLayout;
}


/*
 * Entire Layout for MainWindow
 */
QGridLayout * MainWindow::createMainLayout() {
    QGridLayout * mainLayout = new QGridLayout(this);

    QVBoxLayout * graphicsLayout = createGraphicsLayout();
    QVBoxLayout * buttonLayout = createButtonLayout();

    // Image layout spans 7 of 8 columns, and all rows
    mainLayout->addLayout(graphicsLayout, 0, 0, 10, 7);
    // Button layout spans last column, and all rows
    mainLayout->addLayout(buttonLayout, 0, 7, 10, 1);

    return mainLayout;
}

/*
 * Update graphics with the relevant sizing
 */
QGraphicsPixmapItem * MainWindow::initGraphics() {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    image = QPixmap("images//sampleImage.jpg");

    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);
    view->show();

    return imageItem;
}

/*
 * Rescale picture to be as large as possible while keeping aspect ratio
 */
void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);


    // Remove old image
    scene->removeItem(imageItem);
    delete imageItem;

    // Rescale new image
    image = QPixmap("images//sampleImage.jpg").scaled(
                static_cast<int>(view->size().width()),
                static_cast<int>(view->size().height()),
                Qt::KeepAspectRatioByExpanding,
                Qt::FastTransformation );
    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);
    scene->setSceneRect(0, 0, image.width(), image.height());
    view->show();
}

MainWindow::~MainWindow()
{
}
