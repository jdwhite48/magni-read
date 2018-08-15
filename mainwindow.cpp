#include "mainwindow.h"

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
}

/*
 * Switch from preview (video) to snapshot (image) mode, or vice versa
 * Disable accessing settings when in snapshot mode
 */
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
QHBoxLayout * MainWindow::createButtonLayout() {
     QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    QHBoxLayout * buttonLayout = new QHBoxLayout(this);

    fullscreenButton = new QPushButton(this);
    zoomSlider = new QSlider(Qt::Horizontal, this);
    modeButton = new QPushButton(this);
    settingsButton = new QPushButton(this);

    // Set tooltips and icons for buttons
    fullscreenButton->setToolTip(WINDOW_TOOLTIP);
    settingsButton->setToolTip("Settings");

    // Give the mode button the appropriate icon && tooltip
    updateWebcamMode();

    fullscreenButton->setIcon(QIcon(":/media/icons/fullscreen.svg"));
    settingsButton->setIcon(QIcon(":/media/icons/gear.png"));

    // Artificially widens size alloted for widget for stylesheet
    zoomSlider->setTickPosition(QSlider::TicksBothSides);

    zoomSlider->setMinimum(0);
    int maxZoomPos = (settings.contains("image/maxZoom"))
                   ? settings.value("image/maxZoom").toInt() * 100
                   : 500;
    zoomSlider->setMaximum(maxZoomPos);
    zoomSlider->setTickInterval( int((zoomSlider->maximum() - zoomSlider->minimum())/10) );
    zoomSlider->setSingleStep(1);
    zoomSlider->setPageStep( int((zoomSlider->maximum() - zoomSlider->minimum())/10) );
    zoomSlider->setSliderPosition(zoomSlider->minimum());
    // Track zoom Slider as it's moving
    zoomSlider->setTracking(true);

    QLabel * zoomTitle = new QLabel("Zoom:", this);

    QString label;
    label += QString::number(maxZoomPos/100);
    label += "x";
    maxZoomLabel = new QLabel(label, this);

    QLabel * minZoomLabel = new QLabel("1x", this);

    buttonLayout->addWidget(fullscreenButton);
    buttonLayout->addWidget(zoomTitle);
    buttonLayout->addWidget(minZoomLabel);
    buttonLayout->addWidget(zoomSlider);
    buttonLayout->addWidget(maxZoomLabel);
    buttonLayout->addWidget(modeButton);
    buttonLayout->addWidget(settingsButton);

    // Customize layout

    // Keep buttons fixed to their respective size (defined in stylesheet)
    fullscreenButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    modeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    settingsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Additional customization in stylesheet with these object names
    fullscreenButton->setObjectName("main");
    modeButton->setObjectName("main");
    settingsButton->setObjectName("main");
    zoomTitle->setObjectName("title");

    // "Settings" button opens dialog box for modifying advanced settings
    connect(settingsButton, SIGNAL (released()), this, SLOT (openSettingsDialog()));
    // Change Mode from still image to video feed & vice versa
    connect(modeButton, SIGNAL (released()), this, SLOT (switchWebcamMode()));
    connect(view, SIGNAL (modeChanged()), this, SLOT (updateWebcamMode()), Qt::QueuedConnection);
    connect(zoomSlider, SIGNAL  (valueChanged(int)), this, SLOT (zoomImage(int)));
    connect(fullscreenButton, SIGNAL (released()), this, SLOT (toggleFullscreen()));

    return buttonLayout;
}

/*
 * Layout for displaying graphics and button for advanced image modifications
 */
QVBoxLayout * MainWindow::createGraphicsLayout() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    QVBoxLayout * graphicsLayout = new QVBoxLayout(this);

    view = new WebcamView(this);

    graphicsLayout->addWidget(view);

    // Take up as much screen as possible
    view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // Update view settings
    if (settings.contains("image/brightness")) {
        view->setBrightness(settings.value("image/brightness").toDouble());
    }

    if (settings.contains("image/contrast")) {
        view->setContrast(settings.value("image/contrast").toDouble());
    }

    if (settings.contains("image/colorFilter")) {
        view->setFilter(settings.value("image/colorFilter").toString().toStdString());
    }

    if (settings.contains("image/angle")) {
        view->setRotation( settings.value("image/angle").toInt() );
    }

    return graphicsLayout;
}

/*
 * Entire Layout for MainWindow
 */
QGridLayout * MainWindow::createMainLayout() {
    QGridLayout * mainLayout = new QGridLayout(this);

    QVBoxLayout * graphicsLayout = createGraphicsLayout();
    QHBoxLayout * buttonLayout = createButtonLayout();

    // Graphics layout spans across most of the window
    mainLayout->addLayout(graphicsLayout, 0, 0, 10, 1);
    // Button layout spans across last row
    mainLayout->addLayout(buttonLayout, 10, 0, 1, 1);

    return mainLayout;
}

/*
 * Bring up dialog box for changing advanced settings
 */
void MainWindow::openSettingsDialog() {
    settingsDialog = new SettingsDialog(this);

    connect( settingsDialog, SIGNAL (tempSettingsChanged()), this, SLOT (trySettings()) );
    connect( settingsDialog, SIGNAL (accepted()), this, SLOT (saveSettings()) );

    // Prevent main window from being intseracted with until dialog closed (i.e. make it modal)
    settingsDialog->exec();
}

/*
 * Change settings based on values selected from settings window
 */
void MainWindow::saveSettings() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    // Update webcam
    if (settings.contains("webcam/deviceIndex")) {
        int newWebcam = settings.value("webcam/deviceIndex").toInt();
        if (newWebcam != curWebcam) {
            curWebcam = newWebcam;
            view->openWebcam(newWebcam);
        }
    }

    // Update max zoom label and limit
    if (settings.contains("image/maxZoom")) {
        int maxZoomFactor = settings.value("image/maxZoom").toInt();
        QString label;
        label += QString::number(maxZoomFactor);
        label += "x";
        maxZoomLabel->setText(label);

        zoomSlider->blockSignals(true);
        zoomSlider->setMaximum(100 * maxZoomFactor);
        zoomSlider->blockSignals(false);
    }

    // Update view settings
    if (settings.contains("image/brightness")) {
        view->setBrightness(settings.value("image/brightness").toDouble());
    }

    if (settings.contains("image/contrast")) {
        view->setContrast(settings.value("image/contrast").toDouble());
    }

    if (settings.contains("image/colorFilter")) {
        view->setFilter(settings.value("image/colorFilter").toString().toStdString());
    }

    if (settings.contains("controls/clickToDrag")) {
        view->setClickToDragEnabled( settings.value("controls/clickToDrag").toBool() );
    }

    if (settings.contains("image/angle")) {
        view->setRotation( settings.value("image/angle").toInt() );
    }

    if (settings.contains("controls/isLineDrawn")) {
        view->setGuidingLineEnabled( settings.value("controls/isLineDrawn").toBool() );
    }

    if (settings.contains("controls/linePos")) {
        // Change percentage to fraction of position down the screen
        view->setGuidingLinePos( settings.value("controls/linePos").toDouble() / 100 );
    }

    if (settings.contains("controls/lineThickness")) {
        view->setGuidingLineThickness( settings.value("controls/lineThickness").toInt() );
    }

    if (settings.contains("controls/lineColor")) {
        QColor color = QColor( settings.value("controls/lineColor").toString() );
        if (color.isValid()) {
            view->setGuidingLineColor(color);
        }
    }

    if (view->getMode() == WebcamView::SNAPSHOT) {
    }

    // If previous webcam resulted in an error, try again
    if (view->getMode() == WebcamView::ERROR) {
        view->setMode(WebcamView::PREVIEW);
    }
}

/*
 * Dynamically change image settings while the values are being changed
 */
void MainWindow::trySettings() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    if (settings.contains("image/tempBrightness")) {
        view->setBrightness( settings.value("image/tempBrightness").toDouble() );
    }

    if (settings.contains("image/tempContrast")) {
        view->setContrast( settings.value("image/tempContrast").toDouble() );
    }

    if (settings.contains("image/tempColorFilter")) {
        view->setFilter( settings.value("image/tempColorFilter").toString().toStdString() );
    }

    if (settings.contains("image/tempAngle")) {
        view->setRotation( settings.value("image/tempAngle").toInt() );
    }

    if (settings.contains("controls/tempIsLineDrawn")) {
        view->setGuidingLineEnabled( settings.value("controls/tempIsLineDrawn").toBool() );
    }

    if (settings.contains("controls/tempLinePos")) {
        // Change percentage to fraction of position down the screen
        view->setGuidingLinePos( settings.value("controls/tempLinePos").toDouble() / 100 );
    }

    if (settings.contains("controls/tempLineThickness")) {
        view->setGuidingLineThickness( settings.value("controls/tempLineThickness").toInt() );
    }

    if (settings.contains("controls/tempLineColor")) {
        QColor color = QColor( settings.value("controls/tempLineColor").toString() );
        if (color.isValid()) {
            view->setGuidingLineColor(color);
        }
    }

    if (view->getMode() == WebcamView::SNAPSHOT) {
        view->processSnapshotImage();
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
 * Leave Fullscreen mode if Esc key pressed
 */
void MainWindow::keyPressEvent(QKeyEvent * event) {
    QMainWindow::keyPressEvent(event);

    if (this->isFullScreen() && event->key() == Qt::Key_Escape) {
        toggleFullscreen();
    }
}

/*
 * Toggle between fullscreen and windowed mode
 */
void MainWindow::toggleFullscreen() {
    // Toggle state
    this->setWindowState(windowState() ^ Qt::WindowFullScreen);

    // Set button to return to other state
    if (this->isFullScreen()) {
        fullscreenButton->setToolTip(FULLSCREEN_TOOLTIP);
        fullscreenButton->setIcon(QIcon(":/media/icons/fullscreen-exit.png"));
    }
    else {
        fullscreenButton->setToolTip(WINDOW_TOOLTIP);
        fullscreenButton->setIcon(QIcon(":/media/icons/fullscreen.svg"));
    }
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
