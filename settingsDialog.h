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
#include <QSlider>
#include <QVBoxLayout>

#include "webcamsettings.cpp"

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

public:
    SettingsDialog();
    SettingsDialog(QWidget * parent);

    const struct webcamSettings DEFAULT_SETTINGS = {
        0   // Device (int)
    };

private slots:
    void closeDialog();
    void saveAndCloseDialog();

signals:
    void settingsChanged(webcamSettings settings);

};

#endif // SETTINGSDIALOG_H
