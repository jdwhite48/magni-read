#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QResizeEvent>

#include "webcamview.h"
#include "settingsDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * window = nullptr;
    WebcamView * view = nullptr;
    QPushButton * modeButton;
    QPushButton * fullscreenButton;
    QPushButton * settingsButton;
    QSlider * zoomSlider = nullptr;
    QLabel * maxZoomLabel;

    SettingsDialog * settingsDialog;

    const char * SNAPSHOT_TOOLTIP = "Preview camera";
    const char * PREVIEW_TOOLTIP = "Take a snapshot";
    const char * ERROR_TOOLTIP = "Cannot find camera";
    const char * FULLSCREEN_TOOLTIP = "Return to Window";
    const char * WINDOW_TOOLTIP = "Enter Fullscreen";

    QGridLayout * createMainLayout();
    QVBoxLayout * createGraphicsLayout();
    QHBoxLayout * createButtonLayout();

private slots:
    void openSettingsDialog();
    void updateWebcamMode();
    void saveSettings();
    void switchWebcamMode();
    void toggleFullscreen();
    void trySettings();
    void zoomImage(int value);

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
