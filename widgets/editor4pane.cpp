#include "editor4pane.hpp"

#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGLWidget>

// OpenGL
#include <QGLFormat>
#include <QGLWidget>


Editor4Pane::Editor4Pane(QWidget *parent) : QDockWidget(parent)
{
    this->setWindowTitle("Editor4Pane");

    QWidget *centralWidget = new QWidget(this);
    this->setWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

	// Split the central widget into left/right
	QSplitter *leftRightSplitter = new QSplitter(Qt::Horizontal, centralWidget);
	leftRightSplitter->setChildrenCollapsible(false);
    layout->addWidget(leftRightSplitter);

	// Split the left/right into top/bottom
	QSplitter *leftUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
	leftUpDownSplitter->setChildrenCollapsible(false);
	
	QSplitter *rightUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
	rightUpDownSplitter->setChildrenCollapsible(false);

	// TODO: Synchronize the splitter sizes

    // Top-left is a OpenGL widget
    QGLWidget *glWidget = new QGLWidget(leftUpDownSplitter);

    // some quick test rendering
    glWidget->makeCurrent();
    // clear the screen
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // swap buffers
    glWidget->swapBuffers();
    glWidget->doneCurrent();
    

    // Disable all features
    // UITODO: At some point, this will be floatable, closable, etc.
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}
