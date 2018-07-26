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
#include <QDragMoveEvent>

#include "webcamview.h"
#include "settingsDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * window = nullptr;
    WebcamView * view = nullptr;
    QPushButton * modeButton = nullptr;
    QSlider * zoomSlider = nullptr;

    SettingsDialog * settingsDialog;

    const char * SNAPSHOT_TOOLTIP = "Preview camera";
    const char * PREVIEW_TOOLTIP = "Take a snapshot";
    const char * ERROR_TOOLTIP = "Cannot find camera";
    const int BUTTON_SIZE = 100;

    QGridLayout * createMainLayout();
    QVBoxLayout * createGraphicsLayout();
    QVBoxLayout * createButtonLayout();

private slots:
    void openSettingsDialog();
    void updateWebcamMode();
    void changeSettings();
    void switchWebcamMode();
    void zoomImage(int value);

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
