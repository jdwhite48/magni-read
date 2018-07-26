#include "settingsDialog.h"

#include <QGuiApplication>
#include <QScreen>

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
 * Close dialog box without saving settings
 */
void SettingsDialog::closeDialog() {
    // Emit "rejected" signal (settings not changed) and hide window
    this->reject();
    this->close();
}

/*
 * Design dialog box that displays advanced settings & preview for the webcam view
 */
QVBoxLayout * SettingsDialog::createDialogLayout() {
    QVBoxLayout * dialogLayout = new QVBoxLayout(this);

    QFormLayout * settingsLayout = createSettingsLayout();
    QHBoxLayout * buttonLayout = createButtonLayout();

    dialogLayout->addLayout(settingsLayout);
    dialogLayout->addLayout(buttonLayout);

    return dialogLayout;
}

/*
 * Design display for settings
 */
QFormLayout * SettingsDialog::createSettingsLayout() {
    QFormLayout * settingsLayout = new QFormLayout(this);
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "JDWhite", "MagniRead");

    // Create brightness and contrast sliders that vary from 1 to 100, ticks every quarter, starting in the middle.
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setTickPosition(QSlider::TicksAbove);
    brightnessSlider->setMinimum(0);
    brightnessSlider->setMaximum(100);
    brightnessSlider->setTickInterval( int((brightnessSlider->maximum() - brightnessSlider->minimum())/4) );
    brightnessSlider->setSingleStep(1);
    brightnessSlider->setPageStep(10);
    // Set brightness slider to previous position, or default if else
    int brightnessSliderPos = ( settings.contains("image/brightness") )
            ? int((brightnessSlider->maximum() - brightnessSlider->minimum()) * settings.value("image/brightness").toDouble())
            : int((brightnessSlider->maximum() - brightnessSlider->minimum()) * DEFAULT_BRIGHTNESS);
    brightnessSlider->setSliderPosition( brightnessSliderPos );
    brightnessSlider->setTracking(true);

    contrastSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider->setTickPosition(QSlider::TicksAbove);
    contrastSlider->setMinimum(0);
    contrastSlider->setMaximum(100);
    contrastSlider->setTickInterval( int((contrastSlider->maximum() - contrastSlider->minimum())/4) );
    contrastSlider->setSingleStep(1);
    contrastSlider->setPageStep(10);
    // Set contrast slider to previous position, or default if else
    int contrastSliderPos = ( settings.contains("image/contrast") )
            ? int((contrastSlider->maximum() - contrastSlider->minimum()) * settings.value("image/contrast").toDouble())
            : int((contrastSlider->maximum() - contrastSlider->minimum()) * DEFAULT_CONTRAST);
    contrastSlider->setSliderPosition( contrastSliderPos );
    contrastSlider->setTracking(true);

    // Drop-down box listing available webcams
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

    // Set default to previously used webcam, or to system's default webcam, or first index if else
    if (settings.contains("webcam/deviceIndex") && settings.value("webcam/deviceIndex").toInt() < webcamBox->count()) {
        webcamBox->setCurrentIndex( settings.value("webcam/deviceIndex").toInt() );
    }
    else {
        restoreWebcamDefault();
    }


    // Add to layout
    settingsLayout->addRow("Brightness:", brightnessSlider);
    settingsLayout->addRow("Contrast:", contrastSlider);
    settingsLayout->addRow("Webcam:", webcamBox);

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

    int brightnessRange = brightnessSlider->maximum() - brightnessSlider->minimum();
    brightnessSlider->setSliderPosition( int(brightnessRange * DEFAULT_BRIGHTNESS) );
    int contrastRange = contrastSlider->maximum() - contrastSlider->minimum();
    contrastSlider->setSliderPosition( int(contrastRange * DEFAULT_CONTRAST) );
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

    int brightnessRange = brightnessSlider->maximum() - brightnessSlider->minimum();
    int contrastRange = contrastSlider->maximum() - contrastSlider->minimum();

    settings.setValue("webcam/deviceIndex", webcamBox->currentIndex());
    settings.setValue("image/brightness", double(brightnessSlider->value()) / brightnessRange );
    settings.setValue("image/contrast", double(contrastSlider->value()) / contrastRange );

    // Emit "accepted" signal (settings changed) and hide window
    this->accept();
    this->close();
}
