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

private slots:
    void closeDialog();
    void saveAndCloseDialog();

signals:
    void settingsChanged();

};

#endif // SETTINGSDIALOG_H
