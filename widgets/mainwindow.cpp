#include "mainwindow.hpp"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle(tr("VoxelThingEditorYeah"));
    this->setMinimumSize(800, 600);

    auto testLabel = new QLabel(tr("Test"));
    auto testButton = new QPushButton(tr("Test"));

    auto layout = new QVBoxLayout();
    layout->addWidget(testLabel);
    layout->addWidget(testButton);
    
    auto centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
}