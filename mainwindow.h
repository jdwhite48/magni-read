#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
    QGraphicsPixmapItem image;

    QHBoxLayout * createMenuLayout(QGraphicsPixmapItem * image);
    QVBoxLayout * createImageLayout(QGraphicsPixmapItem * image);
    QVBoxLayout * createButtonLayout();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
