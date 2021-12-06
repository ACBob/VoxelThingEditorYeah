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
    // default to 50%
    leftUpDownSplitter->setSizes(QList<int>() << 50 << 50);
	QSplitter *rightUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
    // default to 50%
    rightUpDownSplitter->setSizes(QList<int>() << 50 << 50);

    m_topLeftView = new RenderWidget(leftUpDownSplitter);
    m_topLeftView->setDispMode(RenderWidget::DispMode::DISP_3D);
    m_topRightView = new RenderWidget(rightUpDownSplitter);
    m_topRightView->setDispMode(RenderWidget::DispMode::DISP_GRID_XZ);
    m_bottomLeftView = new RenderWidget(leftUpDownSplitter);
    m_bottomLeftView->setDispMode(RenderWidget::DispMode::DISP_GRID_XY);
    m_bottomRightView = new RenderWidget(rightUpDownSplitter);
    m_bottomRightView->setDispMode(RenderWidget::DispMode::DISP_GRID_ZY);

    // // Disable all features
    // this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void Editor4Pane::setChunk(CChunk *chunk)
{
    m_topLeftView->setChunk(chunk);
    m_topRightView->setChunk(chunk);
    m_bottomLeftView->setChunk(chunk);
    m_bottomRightView->setChunk(chunk);
}

void Editor4Pane::setTool(CTool *tool)
{
    m_topLeftView->setTool(tool);
    m_topRightView->setTool(tool);
    m_bottomLeftView->setTool(tool);
    m_bottomRightView->setTool(tool);
}