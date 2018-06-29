#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
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

    QPushButton * defaultButton;
    QPushButton * okButton;
    QPushButton * cancelButton;

    QVBoxLayout * createDialogLayout();
    QFormLayout * createSettingsLayout();
    QHBoxLayout * createButtonLayout();

public:
    SettingsDialog();
    SettingsDialog(QWidget * parent);

};

#endif // SETTINGSDIALOG_H
