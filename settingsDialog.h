#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Parent class
#include <QDialog>

// Implementation classes
#include <QCameraInfo>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSlider>
#include <QVBoxLayout>

#include <QGuiApplication>
#include <QLabel>
#include <QScreen>

class SettingsDialog : public QDialog {

    Q_OBJECT

private:
    QSlider * brightnessSlider;
    QSlider * contrastSlider;
    QComboBox * webcamBox;
    QSpinBox * maxZoomBox;

    QPushButton * defaultButton;
    QPushButton * okButton;
    QPushButton * cancelButton;

    QVBoxLayout * createDialogLayout();
    QGridLayout * createSettingsLayout();
    QHBoxLayout * createButtonLayout();

protected:
    const double DEFAULT_BRIGHTNESS = 0;
    const double DEFAULT_CONTRAST = 1;
    const int DEFAULT_ZOOM = 5;

public:
    SettingsDialog();
    SettingsDialog(QWidget * parent);

signals:
    // Indicates when temporary image display values change
    void tempSettingsChanged();

private slots:
    void changeTempImageSettings();
    void closeDialog();
    void saveAndCloseDialog();
    void restoreDefaults();
    void restoreWebcamDefault();

};

#endif // SETTINGSDIALOG_H
