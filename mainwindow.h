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
#include "settingsdialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // For Main window
    QWidget * window;
    WebcamView * view;

    QGridLayout * createMainLayout();
    QVBoxLayout * createGraphicsLayout();
    QVBoxLayout * createButtonLayout();

    SettingsDialog * settingsDialog;


private slots:
        void openSettingsDialog();

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
