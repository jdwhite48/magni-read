#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    window = new QWidget();
    image.setPixmap(QPixmap("images\\sampleImage.jpg"));
    QHBoxLayout * wLayout = createMenuLayout(&image);

    window->setLayout(wLayout);
    this->setCentralWidget(window);

    // Set window title
    this->setWindowTitle(tr("MagniRead"));

    // Resize window to 100% current screen
    resize(QDesktopWidget().availableGeometry(this).size() * 1.0);
}

QVBoxLayout * MainWindow::createButtonLayout() {
    buttonLayout = new QVBoxLayout();

    QLabel * buttonLabel = new QLabel("Buttons:");
    QPushButton * okButton = new QPushButton("OK");

    buttonLayout ->addWidget(buttonLabel);
    buttonLayout->addWidget(okButton);

    return buttonLayout;
}

QVBoxLayout * MainWindow::createImageLayout(QGraphicsPixmapItem * image) {
    imageLayout = new QVBoxLayout();

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    scene->addItem(image);
    view->show();

    QPushButton * settingsButton = new QPushButton("Settings");

    imageLayout->addWidget(view);
    imageLayout->addWidget(settingsButton);

    return imageLayout;
}

QHBoxLayout * MainWindow::createMenuLayout(QGraphicsPixmapItem * image) {
    QHBoxLayout * menuLayout = new QHBoxLayout();

    QVBoxLayout * imageLayout = createImageLayout(image);
    QVBoxLayout * buttonLayout = createButtonLayout();

    menuLayout->addLayout(imageLayout);
    menuLayout->addLayout(buttonLayout);

    return menuLayout;
}

MainWindow::~MainWindow()
{
    delete window;
}
