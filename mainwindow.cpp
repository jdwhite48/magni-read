#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Add centered text to main window
    QWidget * window = new QWidget();
    QHBoxLayout * layout = new QHBoxLayout();

    QLabel * label = new QLabel("Hello, world!");
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
    window->setLayout(layout);

    this->setCentralWidget(window);

    // Set window title
    this->setWindowTitle(tr("MagniRead"));

    // Resize window to 20% current screen
    resize(QDesktopWidget().availableGeometry(this).size() * 0.2);
}

MainWindow::~MainWindow()
{
}
