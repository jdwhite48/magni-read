#include "settingsDialog.h"

SettingsDialog::SettingsDialog() : QDialog()
{
    SettingsDialog(nullptr);
}

SettingsDialog::SettingsDialog(QWidget * parent)
    : QDialog(parent) {
    QVBoxLayout * dialogLayout = createDialogLayout();

    setLayout(dialogLayout);
    setWindowTitle(tr("Advanced Settings"));
}

/*
 * Save temporary settings for dynamically modifying image settings
 */
void SettingsDialog::changeTempImageSettings() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    settings.setValue("image/tempBrightness", double(brightnessSlider->value()) );
    settings.setValue("image/tempContrast", double(contrastSlider->value()) / 100 );

    emit tempSettingsChanged();
}

/*
 * Revert unsaved chantes and close dialog box
 */
void SettingsDialog::closeDialog() {
    // Revert unsaved changes
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");
    if (settings.contains("image/brightness")) {
        settings.setValue("image/tempBrightness", settings.value("image/brightness").toDouble() );
    }

    if (settings.contains("image/contrast")) {
        settings.setValue("image/tempContrast", settings.value("image/contrast").toDouble() );
    }

    emit tempSettingsChanged();

    // Emit "rejected" signal (settings not changed) and hide window
    this->reject();
    this->close();
}

/*
 * Design dialog box that displays advanced settings & preview for the webcam view
 */
QVBoxLayout * SettingsDialog::createDialogLayout() {
    QVBoxLayout * dialogLayout = new QVBoxLayout(this);

    QGridLayout * settingsLayout = createSettingsLayout();
    QHBoxLayout * buttonLayout = createButtonLayout();

    dialogLayout->addLayout(settingsLayout);
    dialogLayout->addLayout(buttonLayout);

    return dialogLayout;
}

/*
 * Design display for settings
 */
QGridLayout * SettingsDialog::createSettingsLayout() {
    QGridLayout * settingsLayout = new QGridLayout(this);
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    // Create brightness and contrast sliders that vary from 1 to 100, ticks every quarter, starting in the middle.
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setTickPosition(QSlider::TicksAbove);
    // Set range from very low to very high (as if percentage)
    brightnessSlider->setMinimum(-200);
    brightnessSlider->setMaximum(200);
    brightnessSlider->setTickInterval( int((brightnessSlider->maximum() - brightnessSlider->minimum()) / 10) ); // i.e. 10%
    brightnessSlider->setSingleStep(1);
    brightnessSlider->setPageStep( int((brightnessSlider->maximum() - brightnessSlider->minimum()) / 10) );

    // Set brightness slider to previous position, or default if else
    int brightnessSliderPos = ( settings.contains("image/brightness") )
            ? int(settings.value("image/brightness").toDouble())
            : int(DEFAULT_BRIGHTNESS);
    brightnessSlider->setSliderPosition( brightnessSliderPos );

    contrastSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider->setTickPosition(QSlider::TicksAbove);
    // Set range from 0.5x to 3x
    contrastSlider->setMinimum(50);
    contrastSlider->setMaximum(300);
    contrastSlider->setTickInterval(50); // i.e. tick every x0.5
    contrastSlider->setSingleStep(1);
    contrastSlider->setPageStep(50);

    // Set contrast slider to previous position, or default if else
    int contrastSliderPos = ( settings.contains("image/contrast") )
            ? int(settings.value("image/contrast").toDouble() * 100)
            : int(DEFAULT_CONTRAST * 100); // Multiply by 100 to convert from double to int scale
    contrastSlider->setSliderPosition( contrastSliderPos );

    // Drop-down list of available webcams
    webcamBox = new QComboBox(this);
    QList<QCameraInfo> webcams = QCameraInfo::availableCameras();
    QList<QString> webcamNames = {};
    if (webcams.count() > 0) {
        // Get list of webcams
        for (QCameraInfo & webcamInfo : webcams) {
            webcamNames << webcamInfo.description();
        }
    }
    webcamBox->addItems(webcamNames);

    // Spin box for max zoom multiplier (2x-20x, default 5x)
    maxZoomBox = new QSpinBox(this);
    maxZoomBox->setRange(2, 20);
    int curZoom = (settings.contains("image/maxZoom")) ? settings.value("image/maxZoom").toInt() : DEFAULT_ZOOM;
    maxZoomBox->setValue(curZoom);
    maxZoomBox->setSingleStep(1);
    maxZoomBox->setSuffix("x");


    // Set default to previously used webcam, or to system's default webcam, or first index if else
    if (settings.contains("webcam/deviceIndex") && settings.value("webcam/deviceIndex").toInt() < webcamBox->count()) {
        webcamBox->setCurrentIndex( settings.value("webcam/deviceIndex").toInt() );
    }
    else {
        restoreWebcamDefault();
    }

    // Construct UI layout for each row

    // Row 1: Brightness
    QLabel * brightnessLabel = new QLabel("Brightness: ", this);
    QIcon brightnessIcon = QIcon(":/media/icons/sunny.png");
    QLabel * brightnessIconLabel = new QLabel(this);
    brightnessIconLabel->setPixmap(brightnessIcon.pixmap(QSize(75, 75)));
    QLabel * minBrightnessLabel = new QLabel("0%", this);
    QLabel * maxBrightnessLabel = new QLabel("100%", this);
    settingsLayout->addWidget(brightnessLabel, 0, 0, Qt::AlignLeft);
    settingsLayout->addWidget(brightnessIconLabel, 0, 1, Qt::AlignHCenter);
    settingsLayout->addWidget(minBrightnessLabel, 0, 2, Qt::AlignHCenter);
    settingsLayout->addWidget(brightnessSlider, 0, 3, 1, 10); // Span most of the row
    settingsLayout->addWidget(maxBrightnessLabel, 0, 13, Qt::AlignHCenter);

    // Row 2: Contrast
    QLabel * contrastLabel = new QLabel("Contrast: ", this);
    QIcon contrastIcon = QIcon(":/media/icons/contrast.png");
    QLabel * contrastIconLabel = new QLabel(this);
    contrastIconLabel->setPixmap(contrastIcon.pixmap(QSize(50, 50)));
    QLabel * minContrastLabel = new QLabel("0.5x", this);
    QLabel * maxContrastLabel = new QLabel("3x", this);
    settingsLayout->addWidget(contrastLabel, 1, 0, Qt::AlignLeft);
    settingsLayout->addWidget(contrastIconLabel, 1, 1, Qt::AlignHCenter);
    settingsLayout->addWidget(minContrastLabel, 1, 2, Qt::AlignHCenter);
    settingsLayout->addWidget(contrastSlider, 1, 3, 1, 10); // Span most of the row
    settingsLayout->addWidget(maxContrastLabel, 1, 13, Qt::AlignHCenter);

    // Row 3: Webcam
    QLabel * webcamLabel = new QLabel("Webcam:", this);
    settingsLayout->addWidget(webcamLabel, 2, 0, Qt::AlignLeft);
    settingsLayout->addWidget(webcamBox, 2, 2, 1, 12); // Span the remaining part of the row

    // Row 4: Zoom
    QLabel * zoomLabel = new QLabel("Max Zoom:", this);
    settingsLayout->addWidget(zoomLabel, 3, 0, Qt::AlignLeft);
    settingsLayout->addWidget(maxZoomBox, 3, 2, 1, 12); // Span the remaining part of the row

    // Modify settings dynamically with slider movement
    brightnessSlider->setTracking(true);
    contrastSlider->setTracking(true);
    connect(brightnessSlider, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()));
    connect(contrastSlider, SIGNAL (valueChanged(int)), this, SLOT (changeTempImageSettings()));

    return settingsLayout;
}



/*
 * Design display for buttons
 */
QHBoxLayout * SettingsDialog::createButtonLayout() {
    QHBoxLayout * buttonLayout = new QHBoxLayout(this);

    defaultButton = new QPushButton("Restore Defaults", this);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    // Add to layout, with "Restore Defaults" on the left, and "Cancel" and "OK" on the right
    buttonLayout->addWidget(defaultButton, 1, Qt::AlignBottom | Qt::AlignLeft);
    // Add spacing equal to 5% the current screen
    buttonLayout->addSpacing( int(QGuiApplication::primaryScreen()->availableGeometry().width() * 0.05));
    buttonLayout->addWidget(cancelButton, 1, Qt::AlignBottom | Qt::AlignRight);
    buttonLayout->addWidget(okButton, 0, Qt:: AlignBottom | Qt::AlignRight);

    // Perform action when buttons pressed
    connect( okButton, SIGNAL (released()), this, SLOT (saveAndCloseDialog()) );
    connect( cancelButton, SIGNAL (released()), this, SLOT (closeDialog()) );
    connect( defaultButton, SIGNAL (released()), this, SLOT (restoreDefaults()) );

    return buttonLayout;

}

/*
 * Change settings back to their default values
 */
void SettingsDialog::restoreDefaults() {
    restoreWebcamDefault();

    brightnessSlider->setSliderPosition( int(DEFAULT_BRIGHTNESS) );
    contrastSlider->setSliderPosition( int(DEFAULT_CONTRAST * 100) );
    maxZoomBox->setValue(DEFAULT_ZOOM);
}

/*
 * Find system default webcam in list (if not found, first index).
 */
void SettingsDialog::restoreWebcamDefault() {
    QList<QCameraInfo> webcams = QCameraInfo::availableCameras();

    if (!QCameraInfo::defaultCamera().isNull()) {
        for (int i = 0; i < webcams.count(); i++) {
            if (webcams[i] == QCameraInfo::defaultCamera()) {
                webcamBox->setCurrentIndex(i);
                break;
            }
        }
    }
    else {
        webcamBox->setCurrentIndex(0);
    }
}

/*
 * Emit that image settings have changed and close dialog box.
 */
void SettingsDialog::saveAndCloseDialog() {
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    // Save settings in native format (Windows: registry, Other: config file)
    settings.setValue("webcam/deviceIndex", webcamBox->currentIndex());
    settings.setValue("image/brightness", double(brightnessSlider->value()));
    settings.setValue("image/contrast", double(contrastSlider->value()) / 100 ); // Divide by 100 to convert from int scale to double
    settings.setValue("image/maxZoom", maxZoomBox->cleanText().toInt());

    // Emit "accepted" signal (settings changed) and hide window
    this->accept();
    this->close();
}
