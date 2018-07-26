#include "mainwindow.h"

#include <QSpacerItem>

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
 * Update UI when mode changes
 */
void MainWindow::updateWebcamMode() {
    switch (view->getMode()) {
        case WebcamView::PREVIEW :
            modeButton->setToolTip(PREVIEW_TOOLTIP);
            modeButton->setIcon(QIcon(":/media/icons/image.png"));
            modeButton->setEnabled(true);
            break;
        case WebcamView::SNAPSHOT :
            modeButton->setToolTip(SNAPSHOT_TOOLTIP);
            modeButton->setIcon(QIcon(":/media/icons/videocam.png"));
            modeButton->setEnabled(true);
            break;
        case WebcamView::ERROR :
        default :
            modeButton->setToolTip(ERROR_TOOLTIP);
            modeButton->setIcon(QIcon(":/media/icons/videocam-off.png"));
            modeButton->setEnabled(false);
            break;
    }
    modeButton->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
}

void MainWindow::switchWebcamMode() {
    switch (view->getMode()) {
        case WebcamView::SNAPSHOT :
            view->setMode(WebcamView::PREVIEW);
            break;
        case WebcamView::PREVIEW :
            view->setMode(WebcamView::SNAPSHOT);
            break;
        case WebcamView::ERROR :
        default :
            // TODO: "Webcam not found" image shown on screen
            break;
    }
}

/*
 * Layout for displaying interactive widgets that change the display
 */
QVBoxLayout * MainWindow::createButtonLayout() {
     QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    QVBoxLayout * buttonLayout = new QVBoxLayout(this);

    QPushButton * fullscreenButton = new QPushButton(this);
    zoomSlider = new QSlider(this);
    modeButton = new QPushButton(this);
    QPushButton * settingsButton = new QPushButton(this);

    // Set tooltips and icons for buttons
    fullscreenButton->setToolTip("Enter Fullscreen");
    settingsButton->setToolTip("Settings");

    // Give the mode button the appropriate icon && tooltip
    updateWebcamMode();

    QSize buttonSize = QSize(BUTTON_SIZE, BUTTON_SIZE);

    fullscreenButton->setIcon(QIcon(":/media/icons/fullscreen.svg"));
    settingsButton->setIcon(QIcon(":/media/icons/gear.png"));
    modeButton->setIconSize(buttonSize);
    fullscreenButton->setIconSize(buttonSize);
    settingsButton->setIconSize(buttonSize);
    modeButton->setFixedWidth(BUTTON_SIZE);
    fullscreenButton->setFixedWidth(BUTTON_SIZE);
    settingsButton->setFixedWidth(BUTTON_SIZE);

    // Customize buttons in stylesheet with "QPushButton#mainButton {...}"
    fullscreenButton->setObjectName("mainButton");
    modeButton->setObjectName("mainButton");
    settingsButton->setObjectName("mainButton");



    zoomSlider->setTickPosition(QSlider::TicksBothSides);
    zoomSlider->setMinimum(0);
    int maxZoom = (settings.contains("image/maxZoom")
                   ? settings.value("image/maxZoom").toInt() * 100
                   : 500);
    zoomSlider->setMaximum(maxZoom);
    zoomSlider->setTickInterval( int((zoomSlider->maximum() - zoomSlider->minimum())/10) );
    zoomSlider->setSingleStep(1);
    zoomSlider->setPageStep( int((zoomSlider->maximum() - zoomSlider->minimum())/10) );
    zoomSlider->setSliderPosition(zoomSlider->minimum());
    zoomSlider->setTracking(true);

    buttonLayout->addWidget(fullscreenButton);
    buttonLayout->addWidget(zoomSlider);
    buttonLayout->setAlignment(zoomSlider,  Qt::AlignHCenter);
    buttonLayout->addWidget(modeButton);
    buttonLayout->addWidget(settingsButton);

    // "Settings" button opens dialog box for modifying advanced settings
    connect(settingsButton, SIGNAL (released()), this, SLOT (openSettingsDialog()));
    // Change Mode from still image to video feed & vice versa
    connect(modeButton, SIGNAL (released()), this, SLOT (switchWebcamMode()));
    connect(view, SIGNAL (modeChanged()), this, SLOT (updateWebcamMode()));
    connect(zoomSlider, SIGNAL  (valueChanged(int)), this, SLOT (zoomImage(int)));

    return buttonLayout;
}

/*
 * Layout for displaying graphics and button for advanced image modifications
 */
QVBoxLayout * MainWindow::createGraphicsLayout() {
    QVBoxLayout * graphicsLayout = new QVBoxLayout(this);

    view = new WebcamView(this);

    graphicsLayout->addWidget(view);

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
 * Bring up dialog box for changing advanced settings
 */
void MainWindow::openSettingsDialog() {
    settingsDialog = new SettingsDialog(this);

    connect( settingsDialog, SIGNAL (accepted()), this, SLOT (changeSettings()) );

    // Prevent main window from being interacted with until dialog closed (i.e. make it modal)
    settingsDialog->exec();
}

/*
 * Change settings based on values selected from settings window
 */
void MainWindow::changeSettings() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    qDebug() << "Brightness: " << settings.value("image/brightness").toDouble();
    qDebug() << "Contrast: " << settings.value("image/contrast").toDouble();

    if (settings.contains("webcam/deviceIndex")) {
        view->openWebcam(settings.value("webcam/deviceIndex").toInt());
    }
    if (settings.contains("image/maxZoom")) {
        zoomSlider->blockSignals(true);
        zoomSlider->setMaximum(100 * settings.value("image/maxZoom").toInt());
        zoomSlider->blockSignals(false);
    }
}

/*
 * Rescale picture to be as large as possible while keeping aspect ratio
 */
void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);

    view->resize();
}

/*
 * Zoom in image as a proportion of the slider
 */
void MainWindow::zoomImage(int zoomValue) {
    double zoomRatio = 1 + double(zoomValue)/100;
    QMatrix matrix;
    matrix.scale(zoomRatio, zoomRatio);
    view->setMatrix(matrix);
}

MainWindow::~MainWindow()
{
}
