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
	m_leftSplitter = new QSplitter( Qt::Vertical, leftRightSplitter );
	m_rightSplitter = new QSplitter( Qt::Vertical, leftRightSplitter );
	
	// default to 50%
	m_leftSplitter->setStretchFactor( 0, 1 );
	m_leftSplitter->setStretchFactor( 1, 1 );

	connect( m_leftSplitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterLeftMoved(int,int)) );
	connect( m_rightSplitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterRightMoved(int,int)) );

	m_topLeftView = new RenderWidget( editorState, m_leftSplitter );
	m_topLeftView->setDispMode( RenderWidget::DispMode::DISP_3D );
	m_topRightView = new RenderWidget( editorState, m_rightSplitter );
	m_topRightView->setDispMode( RenderWidget::DispMode::DISP_GRID_XZ );
	m_bottomLeftView = new RenderWidget( editorState, m_leftSplitter );
	m_bottomLeftView->setDispMode( RenderWidget::DispMode::DISP_GRID_XY );
	m_bottomRightView = new RenderWidget( editorState, m_rightSplitter );
	m_bottomRightView->setDispMode( RenderWidget::DispMode::DISP_GRID_ZY );

	// // Disable all features
	// this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void Editor4Pane::splitterLeftMoved( int pos, int index )
{
	m_rightSplitter->setSizes( QList<int>() << pos << m_rightSplitter->height() - pos );
}

void Editor4Pane::splitterRightMoved( int pos, int index )
{
	m_leftSplitter->setSizes( QList<int>() << pos << m_leftSplitter->height() - pos );
}