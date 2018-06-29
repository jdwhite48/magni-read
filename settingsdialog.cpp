#include "settingsdialog.h"

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

    // Create brightness and contrast sliders that vary from 1 to 100, ticks every quarter, starting in the middle.
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setTickPosition(QSlider::TicksAbove);
    brightnessSlider->setMinimum(0);
    brightnessSlider->setMaximum(100);
    brightnessSlider->setTickInterval( static_cast<int>((brightnessSlider->maximum() - brightnessSlider->minimum())/4) );
    brightnessSlider->setSingleStep(1);
    brightnessSlider->setPageStep(10);
    brightnessSlider->setSliderPosition( static_cast<int>((brightnessSlider->maximum() - brightnessSlider->minimum())/2) );
    brightnessSlider->setTracking(true);

    contrastSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider->setTickPosition(QSlider::TicksAbove);
    contrastSlider->setMinimum(0);
    contrastSlider->setMaximum(100);
    contrastSlider->setTickInterval( static_cast<int>((contrastSlider->maximum() - contrastSlider->minimum())/4) );
    contrastSlider->setSingleStep(1);
    contrastSlider->setPageStep(10);
    contrastSlider->setSliderPosition( static_cast<int>((contrastSlider->maximum() - contrastSlider->minimum())/2) );
    contrastSlider->setTracking(true);

    // Add to layout
    settingsLayout->addRow("Brightness:", brightnessSlider);
    settingsLayout->addRow("Contrast:", contrastSlider);

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

    // Add to layout, with "Restore Defaults" on the left, and "OK" and "Cancel" on the right
    buttonLayout->addWidget(defaultButton, 1, Qt::AlignBottom | Qt::AlignLeft);
    buttonLayout->addStretch(2);
    buttonLayout->addWidget(okButton, 1, Qt::AlignBottom | Qt::AlignRight);
    buttonLayout->addWidget(cancelButton, 0, Qt:: AlignBottom | Qt::AlignRight);

    return buttonLayout;

}
