#include "editor4pane.hpp"

#include <QGLWidget>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

// OpenGL
#include <QGLFormat>
#include <QGLFunctions>
#include <QGLWidget>

#include "ui/widgets/renderwidget.hpp"

#include "world/chunk.hpp"
#include "world/world.hpp"

#include <math.h>

Editor4Pane::Editor4Pane( EditorState *editorState, QWidget *parent ) : QDockWidget( parent )
{
	this->setWindowTitle( "Editor4Pane" );

	m_editorState = editorState;

	QWidget *centralWidget = new QWidget( this );
	this->setWidget( centralWidget );

	QVBoxLayout *layout = new QVBoxLayout( centralWidget );
	centralWidget->setLayout( layout );

	// Split the central widget into left/right
	QSplitter *leftRightSplitter = new QSplitter( Qt::Horizontal, centralWidget );
	layout->addWidget( leftRightSplitter );

	// Split the left/right into top/bottom
	QSplitter *leftUpDownSplitter = new QSplitter( Qt::Vertical, leftRightSplitter );
	// default to 50%
	leftUpDownSplitter->setSizes( QList<int>() << 50 << 50 );
	QSplitter *rightUpDownSplitter = new QSplitter( Qt::Vertical, leftRightSplitter );
	// default to 50%
	rightUpDownSplitter->setSizes( QList<int>() << 50 << 50 );

	m_topLeftView = new RenderWidget( editorState, leftUpDownSplitter );
	m_topLeftView->setDispMode( RenderWidget::DispMode::DISP_3D );
	m_topRightView = new RenderWidget( editorState, rightUpDownSplitter );
	m_topRightView->setDispMode( RenderWidget::DispMode::DISP_GRID_XZ );
	m_bottomLeftView = new RenderWidget( editorState, leftUpDownSplitter );
	m_bottomLeftView->setDispMode( RenderWidget::DispMode::DISP_GRID_XY );
	m_bottomRightView = new RenderWidget( editorState, rightUpDownSplitter );
	m_bottomRightView->setDispMode( RenderWidget::DispMode::DISP_GRID_ZY );

	// // Disable all features
	// this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}