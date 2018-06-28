#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Main window
    QWidget * window;
    QHBoxLayout * mainLayout;
    QVBoxLayout * graphicsLayout;
    QVBoxLayout * buttonLayout;

    QGraphicsScene * scene;
    QGraphicsView * view;
    QPixmap image;
    QGraphicsPixmapItem * imageItem;


    QGridLayout * createMainLayout();
    QVBoxLayout * createGraphicsLayout();
    QVBoxLayout * createButtonLayout();
    QGraphicsPixmapItem * initGraphics();

    // Settings dialog box
    QDialog * settingsWindow;
    QSlider * brightnessSlider;
    QSlider * contrastSlider;

    QPushButton * defaultButton;
    QPushButton * okButton;
    QPushButton * cancelButton;


private slots:
        void openSettingsDialog();

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
