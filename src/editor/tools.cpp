#include "tools.hpp"

#include "ui/dialogs/blockproperties.hpp"
#include "ui/widgets/renderwidget.hpp"

#include "world/chunk.hpp"
#include "world/raycast.hpp"
#include "world/world.hpp"

#include "defs.hpp"

#include <QDebug>

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

#include <GL/glu.h>
#include <QOpenGLFunctions>

#include "editorstate.hpp"

// How far the tools will cast rays
#define TOOL_CAST_DISTANCE 150.0f

CTool::CTool( EditorState *editorState, QObject *parent ) : QObject( parent ) { m_editorState = editorState; };
CTool::~CTool(){};

CHandTool::CHandTool( EditorState *editorState, QObject *parent ) : CTool( editorState, parent )
{
	m_selectedBlockPos = Vector3f( 0, 0, 0 );
}

CHandTool::~CHandTool() {}

void CHandTool::mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Hand tool pressed @" << pos << "pointing" << dir;
	// m_selectedBlockPos = pos;
	// view->update();

	CRaycast caster;
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->world, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	if ( m_selectedBlockNormal.Magnitude() > 0.0f )
	{
		if ( event->button() == Qt::LeftButton )
		{
			Vector3f p = m_selectedBlockPos;
			m_editorState->world->setID( p.x, p.y, p.z, 0 );
			m_editorState->world->setMeta( p.x, p.y, p.z, 0 );

			// find chunk
			CChunk *c = m_editorState->world->getChunkWorldPos( p );
			c->rebuildModel();
		}
		else if ( event->button() == Qt::RightButton )
		{
			Vector3f p = m_selectedBlockPos + m_selectedBlockNormal;
			m_editorState->world->setID( p.x, p.y, p.z, m_editorState->chosenBlockType );
			m_editorState->world->setMeta( p.x, p.y, p.z, m_editorState->chosenBlockMeta );
			CChunk *c = m_editorState->world->getChunkWorldPos( p );
			c->rebuildModel();
		}
		else if ( event->button() == Qt::MiddleButton )
		{
			Vector3f p = m_selectedBlockPos;
			uint16_t id, meta;
			m_editorState->world->get( p.x, p.y, p.z, id, meta );
			m_editorState->setChosenBlockType( id );
			m_editorState->setChosenBlockMeta( meta );
		}
	}

	view->update();
}

void CHandTool::mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	// qDebug () << "Hand tool moved @" << pos;

	CRaycast caster;
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->world, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	view->update();
}

void CHandTool::wheelEvent( QWheelEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Hand tool wheeled @" << pos;

	if ( event->delta() > 0 )
	{
		m_editorState->chosenBlockType++;
		if ( m_editorState->chosenBlockType >= m_editorState->blockDefs->keys().size() )
			m_editorState->chosenBlockType = 1;
		m_editorState->setChosenBlockType( m_editorState->chosenBlockType ); // HACK: emit the signal
	}
	else
	{
		m_editorState->chosenBlockType--;
		if ( m_editorState->chosenBlockType < 1 )
			m_editorState->chosenBlockType = m_editorState->blockDefs->keys().size() - 1;
		m_editorState->setChosenBlockType( m_editorState->chosenBlockType ); // HACK: emit the signal
	}
}

const static Vector3f cubeLinesVertices[32] = {
	// Bottom
	Vector3f( 0.0f, 0.0f, 0.0f ),
	Vector3f( 1.0f, 0.0f, 0.0f ),
	Vector3f( 0.0f, 0.0f, 0.0f ),
	Vector3f( 0.0f, 0.0f, 1.0f ),
	Vector3f( 1.0f, 0.0f, 0.0f ),
	Vector3f( 1.0f, 0.0f, 1.0f ),
	Vector3f( 0.0f, 0.0f, 1.0f ),
	Vector3f( 1.0f, 0.0f, 1.0f ),

	// Top
	Vector3f( 0.0f, 1.0f, 0.0f ),
	Vector3f( 1.0f, 1.0f, 0.0f ),
	Vector3f( 0.0f, 1.0f, 0.0f ),
	Vector3f( 0.0f, 1.0f, 1.0f ),
	Vector3f( 1.0f, 1.0f, 0.0f ),
	Vector3f( 1.0f, 1.0f, 1.0f ),
	Vector3f( 0.0f, 1.0f, 1.0f ),
	Vector3f( 1.0f, 1.0f, 1.0f ),

	// Sides
	Vector3f( 0.0f, 0.0f, 0.0f ),
	Vector3f( 0.0f, 1.0f, 0.0f ),
	Vector3f( 1.0f, 0.0f, 0.0f ),
	Vector3f( 1.0f, 1.0f, 0.0f ),

	Vector3f( 0.0f, 0.0f, 1.0f ),
	Vector3f( 0.0f, 1.0f, 1.0f ),
	Vector3f( 1.0f, 0.0f, 1.0f ),
	Vector3f( 1.0f, 1.0f, 1.0f ),

	Vector3f( 0.0f, 0.0f, 0.0f ),
	Vector3f( 0.0f, 0.0f, 1.0f ),
	Vector3f( 1.0f, 0.0f, 0.0f ),
	Vector3f( 1.0f, 0.0f, 1.0f ),

	Vector3f( 0.0f, 1.0f, 0.0f ),
	Vector3f( 0.0f, 1.0f, 1.0f ),
	Vector3f( 1.0f, 1.0f, 0.0f ),
	Vector3f( 1.0f, 1.0f, 1.0f ),
};

void CHandTool::draw( RenderWidget *view )
{
	// qDebug () << "Hand tool draw";
	// if we have a pointed voxel, draw a wireframe cube around it
	if ( m_selectedBlockPos != Vector3f( 0, 0, 0 ) )
	{
		glBegin( GL_LINES );
		glLineWidth( 15.0f );
		glColor4f( 0.0f, 0.0f, 1.0f, 0.5f );

		for ( int i = 0; i < 32; i += 2 )
		{
			Vector3f p1 = m_selectedBlockPos + cubeLinesVertices[i];
			Vector3f p2 = m_selectedBlockPos + cubeLinesVertices[i + 1];

			glVertex3f( p1.x, p1.y, p1.z );
			glVertex3f( p2.x, p2.y, p2.z );
		}

		glEnd();
	}
}

// Wrench
CWrenchTool::CWrenchTool( EditorState *editorState, QObject *parent ) : CTool( editorState, parent ) {}

CWrenchTool::~CWrenchTool() {}

void CWrenchTool::mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Wrench tool pressed @" << pos;
	// m_selectedBlockPos = pos;
	// view->update();

	CRaycast caster;
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->world, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	if ( m_selectedBlockNormal.Magnitude() > 0.0f )
	{
		if ( event->button() == Qt::LeftButton )
		{
			if ( m_selectedBlockNormal != Vector3f( 0, 0, 0 ) )
			{
				// Display configure dialog
				uint16_t id, meta;
				m_editorState->world->get( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z, id, meta );
				BlockPropertyDialog *dlg = new BlockPropertyDialog( m_editorState->blockDefs, id, meta );

				if ( dlg->exec() == QDialog::Accepted )
				{
					id	 = dlg->getChosenId();
					meta = dlg->getChosenMeta();

					m_editorState->world->set( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z, id,
											   meta );
					m_editorState->world->getChunkWorldPos( m_selectedBlockPos )->rebuildModel();
					view->update();
				}
			}
		}
	}

	view->update();
}

void CWrenchTool::mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	// qDebug () << "Wrench tool moved @" << pos;

	CRaycast caster;
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->world, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	view->update();
}

void CWrenchTool::draw( RenderWidget *view )
{
	// qDebug () << "Wrench tool draw";
	// if we have a pointed voxel, draw a wireframe cube around it
	if ( m_selectedBlockPos != Vector3f( 0, 0, 0 ) )
	{
		glBegin( GL_LINES );
		glLineWidth( 15.0f );
		glColor4f( 0.0f, 0.0f, 1.0f, 0.5f );

		for ( int i = 0; i < 32; i += 2 )
		{
			Vector3f p1 = m_selectedBlockPos + cubeLinesVertices[i];
			Vector3f p2 = m_selectedBlockPos + cubeLinesVertices[i + 1];

			glVertex3f( p1.x, p1.y, p1.z );
			glVertex3f( p2.x, p2.y, p2.z );
		}

		glEnd();
	}
}

CSimulateTool::CSimulateTool( EditorState *editorState, QObject *parent ) : CTool( editorState, parent ) {}
CSimulateTool::~CSimulateTool() {}

void CSimulateTool::mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Simulate tool pressed @" << pos;

	// cast a ray to find the block we're pointing at
	CRaycast caster;
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->world, pos, dir, TOOL_CAST_DISTANCE );

	// find the chunk the block is in
	CChunk *chunk = m_editorState->world->getChunkWorldPos( cast.first );

	if ( chunk != nullptr )
	{
		if ( event->button() == Qt::LeftButton )
		{
			chunk->simulateLiquid();
			chunk->rebuildModel();
			view->update();
		}
		else if ( event->button() == Qt::RightButton )
		{
			// right button shows a dialog to choose how many iterations to run
			QDialog *dialog = new QDialog( view );
			dialog->setWindowTitle( tr( "Simulation" ) );
			dialog->setWindowModality( Qt::WindowModal );

			QVBoxLayout *layout = new QVBoxLayout( dialog );
			dialog->setLayout( layout );

			QLabel *label = new QLabel( tr( "Iterations" ), dialog );
			layout->addWidget( label );

			QSpinBox *spinBox = new QSpinBox( dialog );
			spinBox->setMinimum( 1 );
			spinBox->setMaximum( 100 );
			layout->addWidget( spinBox );

			QPushButton *okButton = new QPushButton( tr( "Simulate" ), dialog );
			layout->addWidget( okButton );
			QObject::connect( okButton, SIGNAL( clicked() ), dialog, SLOT( accept() ) );

			QPushButton *cancelButton = new QPushButton( tr( "Cancel" ), dialog );
			layout->addWidget( cancelButton );
			QObject::connect( cancelButton, SIGNAL( clicked() ), dialog, SLOT( reject() ) );

			if ( dialog->exec() == QDialog::Accepted )
			{
				int iterations = spinBox->value();
				while ( iterations-- )
				{
					chunk->simulateLiquid();
				}
				chunk->rebuildModel();
				view->update();
			}
		}
	}
}