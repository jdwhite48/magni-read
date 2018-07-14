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
#include "webcamsettings.cpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * window = nullptr;
    WebcamView * view = nullptr;
    QPushButton * modeButton = nullptr;
    QSlider * zoomSlider = nullptr;

    SettingsDialog * settingsDialog;

    const char * SNAPSHOT_TOOLTIP = "Take a snapshot";
    const char * PREVIEW_TOOLTIP = "Preview camera";

    QGridLayout * createMainLayout();
    QVBoxLayout * createGraphicsLayout();
    QVBoxLayout * createButtonLayout();

private slots:
    void openSettingsDialog();
    void switchWebcamMode();
    void processImage(webcamSettings settings);

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
