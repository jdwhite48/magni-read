#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
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

protected:
    void resizeEvent(QResizeEvent * event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
