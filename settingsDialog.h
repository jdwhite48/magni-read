#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Parent class
#include <QDialog>

// Implementation classes
#include <QCameraInfo>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QVBoxLayout>

class SettingsDialog : public QDialog {

    Q_OBJECT

private:
    QSlider * brightnessSlider;
    QSlider * contrastSlider;
    QComboBox * webcamBox;

    QPushButton * defaultButton;
    QPushButton * okButton;
    QPushButton * cancelButton;

    QVBoxLayout * createDialogLayout();
    QFormLayout * createSettingsLayout();
    QHBoxLayout * createButtonLayout();

protected:
    const double DEFAULT_BRIGHTNESS = 0.5;
    const double DEFAULT_CONTRAST = 0.5;

public:
    SettingsDialog();
    SettingsDialog(QWidget * parent);

private slots:
    void closeDialog();
    void saveAndCloseDialog();
    void restoreDefaults();
    void restoreWebcamDefault();

};

#endif // SETTINGSDIALOG_H
