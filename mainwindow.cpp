#include "mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    window = new QWidget();
    QGridLayout * wLayout = createMenuLayout();

    window->setLayout(wLayout);
    this->setCentralWidget(window);

    // Set window title
    this->setWindowTitle(tr("MagniRead"));

    // Maximize window
    this->setWindowState(Qt::WindowMaximized);
}

QVBoxLayout * MainWindow::createButtonLayout() {
    buttonLayout = new QVBoxLayout();

    QLabel * buttonLabel = new QLabel("Buttons:");
    QPushButton * okButton = new QPushButton("OK");

    buttonLayout ->addWidget(buttonLabel);
    buttonLayout->addWidget(okButton);

    return buttonLayout;
}

QVBoxLayout * MainWindow::createImageLayout() {
    imageLayout = new QVBoxLayout();

    drawImage();

    QPushButton * settingsButton = new QPushButton("Settings");

    imageLayout->addWidget(view);
    imageLayout->addWidget(settingsButton);

    return imageLayout;
}

QGridLayout * MainWindow::createMenuLayout() {
    QGridLayout * menuLayout = new QGridLayout(this);

    QVBoxLayout * imageLayout = createImageLayout();
    QVBoxLayout * buttonLayout = createButtonLayout();

    // Image layout spans 7 of 8 columns, and all rows
    menuLayout->addLayout(imageLayout, 0, 0, 10, 7);
    // Button layout spans last column, and all rows
    menuLayout->addLayout(buttonLayout, 0, 7, 10, 1);

    return menuLayout;
}

/*
 * Update QGraphicsView with the relevant sizing and image
 */
QGraphicsPixmapItem * MainWindow::drawImage() {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    image = QPixmap("images//sampleImage.jpg");

    imageItem = new QGraphicsPixmapItem(image);
    scene->addItem(imageItem);
    view->show();

    return imageItem;
}

MainWindow::~MainWindow()
{
    delete window;
}
