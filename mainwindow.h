#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget * window;
    QHBoxLayout * menuLayout;
    QVBoxLayout * imageLayout;
    QVBoxLayout * buttonLayout;

    QGraphicsScene * scene;
    QGraphicsView * view;
    QPixmap image;
    QGraphicsPixmapItem * imageItem;

    QGridLayout * createMenuLayout();
    QVBoxLayout * createImageLayout();
    QVBoxLayout * createButtonLayout();
    QGraphicsPixmapItem * drawImage();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
