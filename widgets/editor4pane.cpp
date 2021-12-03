#include "editor4pane.hpp"

#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGLWidget>

// OpenGL
#include <QGLFormat>
#include <QGLWidget>
#include <QGLFunctions>

#include "renderwidget.hpp"

#include <math.h>


Editor4Pane::Editor4Pane(QWidget *parent) : QDockWidget(parent)
{
    this->setWindowTitle("Editor4Pane");

    QWidget *centralWidget = new QWidget(this);
    this->setWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

	// Split the central widget into left/right
	QSplitter *leftRightSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    layout->addWidget(leftRightSplitter);

	// Split the left/right into top/bottom
	QSplitter *leftUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
	
	QSplitter *rightUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);

	// TODO: Synchronize the splitter sizes

    // Top-left is the viewport
    RenderWidget *viewPort = new RenderWidget(leftUpDownSplitter);

    // The rest are also renderwidgets, but they are for the grid
    RenderWidget *gridXY = new RenderWidget(leftUpDownSplitter);
    RenderWidget *gridXZ = new RenderWidget(rightUpDownSplitter);
    RenderWidget *gridYZ = new RenderWidget(rightUpDownSplitter);

    // Disable all features
    // UITODO: At some point, this will be floatable, closable, etc.
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}
