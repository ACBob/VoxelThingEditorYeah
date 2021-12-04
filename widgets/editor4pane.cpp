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

    // Top-left is the viewport
    m_viewport = new RenderWidget(leftUpDownSplitter);

    // Then we have grid views
    m_gridXY = new RenderWidget(leftUpDownSplitter);
    m_gridXY->setDispMode(RenderWidget::DISP_GRID_XY);
    m_gridXZ = new RenderWidget(rightUpDownSplitter);
    m_gridXZ->setDispMode(RenderWidget::DISP_GRID_XZ);
    m_gridYZ = new RenderWidget(rightUpDownSplitter);
    m_gridYZ->setDispMode(RenderWidget::DISP_GRID_YZ);

    // Disable all features
    // UITODO: At some point, this will be floatable, closable, etc.
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void Editor4Pane::setChunk(CChunk *chunk)
{
    m_viewport->setChunk(chunk);
    m_gridXY->setChunk(chunk);
    m_gridXZ->setChunk(chunk);
    m_gridYZ->setChunk(chunk);
}