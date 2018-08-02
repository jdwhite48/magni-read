#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Parent class
#include <QDialog>

// Implementation classes
#include <QCameraInfo>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSlider>
#include <QVBoxLayout>
#include <string>

#include <QGuiApplication>
#include <QLabel>
#include <QScreen>

class SettingsDialog : public QDialog {

    Q_OBJECT

private:
    QSlider * brightnessSlider;
    QSlider * contrastSlider;
    QComboBox * webcamBox;
    QComboBox * colorFilterBox;
    QSpinBox * rotateAngleBox;
    QSpinBox * maxZoomBox;
    QCheckBox * clickDragBox;

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
    const QString DEFAULT_FILTER = "None";
    const int DEFAULT_ANGLE = 0;
    const bool DEFAULT_CLICK_TO_DRAG = false;

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
