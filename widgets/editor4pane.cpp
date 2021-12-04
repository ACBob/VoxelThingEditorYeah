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
#include "gridwidget.hpp"

#include "../world/chunk.hpp"

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
    m_viewport = new RenderWidget(leftUpDownSplitter);

    // Then we have grid views
    m_gridXY = new GridWidget(leftUpDownSplitter);
    m_gridXZ = new GridWidget(rightUpDownSplitter);
    m_gridYZ = new GridWidget(rightUpDownSplitter);

    // Disable all features
    // UITODO: At some point, this will be floatable, closable, etc.
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void Editor4Pane::setChunk(CChunk *chunk)
{
    m_viewport->setChunk(chunk);
    // m_gridXY->setChunk(chunk);
    // m_gridXZ->setChunk(chunk);
    // m_gridYZ->setChunk(chunk);
}