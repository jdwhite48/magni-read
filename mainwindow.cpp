#include "mainwindow.h"

#include <iostream>
#include <cmath>
#include <QSpacerItem>

/*
 * Default settings for the image
 */
struct {
    bool fullscreen = false;
    bool preview = true;
    double zoom = 1.00;
    double brightness = 0.50;
    double contrast = 0.50;
} DEFAULT_SETTINGS, graphicsSettings;

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

    view = new WebcamView(this);

    QPushButton * settingsButton = new QPushButton("Settings", this);

    graphicsLayout->addWidget(view);
    graphicsLayout->addWidget(settingsButton);

    connect(settingsButton, SIGNAL (released()), this, SLOT (openSettingsDialog()));

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
 * Click to turn scroll dragging on or off for the graphics view
 */
void MainWindow::mousePressEvent(QMouseEvent * event) {
    std::cout << "Mouse pressed" << std::endl;
    // Start dragging when clicking with open hand
    if (view->cursor().shape() == Qt::OpenHandCursor && view->underMouse()) {
        view->setCursor(Qt::ClosedHandCursor);
        view->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    // Stop dragging when clicking with closed hand
    else if (view->cursor().shape() == Qt::ClosedHandCursor && view->underMouse()) {
        view->setCursor(Qt::OpenHandCursor);
        view->setDragMode(QGraphicsView::NoDrag);
    }
    else {
        QMainWindow::mousePressEvent(event);
    }
}

/*
 * Open Dialog box for changing advanced settings
 */
void MainWindow::openSettingsDialog() {
    settingsWindow = new QDialog(this);

    QVBoxLayout * dialogLayout = new QVBoxLayout(settingsWindow);

    QFormLayout * settingsLayout = new QFormLayout(settingsWindow);
    QHBoxLayout * sButtonLayout = new QHBoxLayout(settingsWindow);


    //Create brightness and contrast sliders that vary from 1 to 100, ticks every 25, starting at 50.
    brightnessSlider = new QSlider(Qt::Horizontal, settingsWindow);
    contrastSlider = new QSlider(Qt::Horizontal, settingsWindow);
    brightnessSlider->setTickPosition(QSlider::TicksAbove);
    contrastSlider->setTickPosition(QSlider::TicksAbove);
    brightnessSlider->setMinimum(0);
    brightnessSlider->setMaximum(100);
    contrastSlider->setMinimum(0);
    contrastSlider->setMaximum(100);
    brightnessSlider->setTickInterval(25);
    contrastSlider->setTickInterval(25);
    brightnessSlider->setSingleStep(1);
    contrastSlider->setSingleStep(1);
    brightnessSlider->setPageStep(10);
    contrastSlider->setPageStep(10);
    brightnessSlider->setSliderPosition(50);
    contrastSlider->setSliderPosition(50);
    brightnessSlider->setTracking(true);
    contrastSlider->setTracking(true);

    //Add to QFormLayout
    settingsLayout->addRow("Brightness:", brightnessSlider);
    settingsLayout->addRow("Contrast:", contrastSlider);

    defaultButton = new QPushButton("Restore Defaults", settingsWindow);
    okButton = new QPushButton("OK", settingsWindow);
    cancelButton = new QPushButton("Cancel", settingsWindow);

    //Add to QHBoxLayout
    sButtonLayout->addWidget(defaultButton, 1, Qt::AlignBottom | Qt::AlignLeft);
    sButtonLayout->addStretch(2);
    sButtonLayout->addWidget(okButton, 1, Qt::AlignBottom | Qt::AlignRight);
    sButtonLayout->addWidget(cancelButton, 0, Qt:: AlignBottom | Qt::AlignRight);

    dialogLayout->addLayout(settingsLayout);
    dialogLayout->addLayout(sButtonLayout);

    settingsWindow->setLayout(dialogLayout);
    settingsWindow->setWindowTitle(tr("Advanced Settings"));

    // Prevent main window from being interacted with until dialog closed
    settingsWindow->exec();
}

/*
 * Rescale picture to be as large as possible while keeping aspect ratio
 */
void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);

    view->rescaleImage();
}

MainWindow::~MainWindow()
{
}
