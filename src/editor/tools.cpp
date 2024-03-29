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
#include <QUndoStack>

#include <GL/glu.h>
#include <QOpenGLFunctions>

#include "editorstate.hpp"
#include "undo.hpp"

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
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	if ( m_selectedBlockNormal.Magnitude() > 0.0f )
	{
		if ( event->button() == Qt::LeftButton )
		{
			Vector3f p = m_selectedBlockPos;
			uint32_t oldVoxel = m_editorState->m_pWorld->get( p.x, p.y, p.z );
			m_editorState->m_pWorld->setID( p.x, p.y, p.z, 0 );
			m_editorState->m_pWorld->setMeta( p.x, p.y, p.z, 0 );

			// find chunk
			CChunk *c = m_editorState->m_pWorld->getChunkWorldPos( p );
			c->rebuildModel();

			// push the undo
			UndoBlockEdit *undo = new UndoBlockEdit( m_editorState, p.x, p.y, p.z, oldVoxel, 0 );
			m_editorState->m_pUndoStack->push( undo );
		}
		else if ( event->button() == Qt::RightButton )
		{
			Vector3f p = m_selectedBlockPos + m_selectedBlockNormal;
			uint32_t oldVoxel = m_editorState->m_pWorld->get( p.x, p.y, p.z );
			m_editorState->m_pWorld->setID( p.x, p.y, p.z, m_editorState->m_nChosenBlockType );
			m_editorState->m_pWorld->setMeta( p.x, p.y, p.z, m_editorState->m_nChosenBlockMeta );
			CChunk *c = m_editorState->m_pWorld->getChunkWorldPos( p );
			c->rebuildModel();

			// push the undo
			UndoBlockEdit *undo = new UndoBlockEdit( m_editorState, p.x, p.y, p.z, oldVoxel, m_editorState->m_nChosenBlockType );
			m_editorState->m_pUndoStack->push( undo );
		}
		else if ( event->button() == Qt::MiddleButton )
		{
			Vector3f p = m_selectedBlockPos;
			uint16_t id, meta;
			m_editorState->m_pWorld->get( p.x, p.y, p.z, id, meta );
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
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

	m_selectedBlockPos	  = cast.first;
	m_selectedBlockNormal = cast.second;

	view->update();
}

void CHandTool::wheelEvent( QWheelEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Hand tool wheeled @" << pos;

	if ( event->delta() > 0 )
	{
		m_editorState->m_nChosenBlockType++;
		if ( m_editorState->m_nChosenBlockType >= m_editorState->m_pBlockDefs->keys().size() )
			m_editorState->m_nChosenBlockType = 1;
		m_editorState->setChosenBlockType( m_editorState->m_nChosenBlockType ); // HACK: emit the signal
	}
	else
	{
		m_editorState->m_nChosenBlockType--;
		if ( m_editorState->m_nChosenBlockType < 1 )
			m_editorState->m_nChosenBlockType = m_editorState->m_pBlockDefs->keys().size() - 1;
		m_editorState->setChosenBlockType( m_editorState->m_nChosenBlockType ); // HACK: emit the signal
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
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

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
				m_editorState->m_pWorld->get( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z, id, meta );
				BlockPropertyDialog *dlg = new BlockPropertyDialog( m_editorState->m_pBlockDefs, id, meta );

				if ( dlg->exec() == QDialog::Accepted )
				{
					uint32_t oldVoxel = m_editorState->m_pWorld->get( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z );
					id	 = dlg->getChosenId();
					meta = dlg->getChosenMeta();

					m_editorState->m_pWorld->set( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z, id,
											   meta );
					m_editorState->m_pWorld->getChunkWorldPos( m_selectedBlockPos )->rebuildModel();
					view->update();

					uint32_t newVoxel = m_editorState->m_pWorld->get( m_selectedBlockPos.x, m_selectedBlockPos.y, m_selectedBlockPos.z );

					// Push undo
					UndoBlockEdit *undo = new UndoBlockEdit( m_editorState, m_selectedBlockPos.x, m_selectedBlockPos.y,
															  m_selectedBlockPos.z, oldVoxel, newVoxel );
					m_editorState->m_pUndoStack->push( undo );
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
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

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
	std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

	// find the chunk the block is in
	CChunk *chunk = m_editorState->m_pWorld->getChunkWorldPos( cast.first );

	if ( chunk != nullptr )
	{
		uint32_t *oldVoxels = new uint32_t[chunk->getSizeX() * chunk->getSizeY() * chunk->getSizeZ()];
		uint32_t *newVoxels = new uint32_t[chunk->getSizeX() * chunk->getSizeY() * chunk->getSizeZ()];

		// copy the old voxels
		for ( int i = 0; i < chunk->getSizeX() * chunk->getSizeY() * chunk->getSizeZ(); i++ )
		{
			chunk->get(i, oldVoxels[i]);
		}

		if ( event->button() == Qt::LeftButton )
		{
			chunk->simulateLiquid();
			chunk->rebuildModel();
			view->update();

			// copy the new voxels
			for ( int i = 0; i < chunk->getSizeX() * chunk->getSizeY() * chunk->getSizeZ(); i++ )
			{
				chunk->get(i, newVoxels[i]);
			}

			// from there the undo will delete the arrays when it doesn't need them anymore
			UndoChunkEdit *undo = new UndoChunkEdit( m_editorState, chunk, oldVoxels, newVoxels );
			m_editorState->m_pUndoStack->push( undo );
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


				// copy the new voxels
				for ( int i = 0; i < chunk->getSizeX() * chunk->getSizeY() * chunk->getSizeZ(); i++ )
				{
					chunk->get(i, newVoxels[i]);
				}

				// from there the undo will delete the arrays when it doesn't need them anymore
				UndoChunkEdit *undo = new UndoChunkEdit( m_editorState, chunk, oldVoxels, newVoxels );
				m_editorState->m_pUndoStack->push( undo );
			}
		}
		else
		{
			delete[] oldVoxels;
			delete[] newVoxels;
		}
	}
}

// ----------------------------------------------------------------------------
// Selection tool
// Modifies the editor's selection area and potentially the selected entities
// ----------------------------------------------------------------------------

CSelectTool::CSelectTool( EditorState *editorState, QObject *parent ) : CTool( editorState, parent ) {}
CSelectTool::~CSelectTool() {}

void CSelectTool::mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	qDebug() << "Select tool pressed @" << pos;

	// No need to cast rays in 2D views
	Vector3i selPos = m_editorState->m_vecSelectionAreaStart;
	if (event->button() == Qt::RightButton)
		selPos = m_editorState->m_vecSelectionAreaEnd;	

	if ( view->getDispMode() == RenderWidget::DispMode::DISP_3D || view->getDispMode() == RenderWidget::DispMode::DISP_ISOMETRIC )
	{
		CRaycast caster;
		std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

		selPos	= cast.first;
	}
	else
	{
		// Depending on the view, change the coordinates of the selection

		switch ( view->getDispMode() )
		{
			case RenderWidget::DispMode::DISP_GRID_XY:
				selPos.x = (int)pos.x;
				selPos.y = (int)pos.y;
				break;
			case RenderWidget::DispMode::DISP_GRID_XZ:
				selPos.x = (int)pos.x;
				selPos.z = (int)pos.z;
				break;
			case RenderWidget::DispMode::DISP_GRID_ZY:
				selPos.z = (int)pos.z;
				selPos.y = (int)pos.y;
				break;
		}
	}

	if ( event->button() == Qt::LeftButton )
	{
		m_editorState->m_vecSelectionAreaStart = selPos;
	}
	else if ( event->button() == Qt::RightButton )
	{
		m_editorState->m_vecSelectionAreaEnd = selPos;
	}

	view->update();
}

void CSelectTool::mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view )
{
	// qDebug() << "Select tool moved @" << pos;

	// Only continue if the mouse is held
	if ( event->buttons() & Qt::LeftButton )
	{
		// No need to cast rays in 2D views
		Vector3i selPosEnd = m_editorState->m_vecSelectionAreaEnd;

		if ( view->getDispMode() == RenderWidget::DispMode::DISP_3D || view->getDispMode() == RenderWidget::DispMode::DISP_ISOMETRIC )
		{
			CRaycast caster;
			std::pair<Vector3f, Vector3f> cast = caster.cast( m_editorState->m_pWorld, pos, dir, TOOL_CAST_DISTANCE );

			selPosEnd	= cast.first;
		}
		else
		{
			// Depending on the view, change the coordinates of the selection

			switch ( view->getDispMode() )
			{
				case RenderWidget::DispMode::DISP_GRID_XY:
					selPosEnd.x = (int)pos.x;
					selPosEnd.y = (int)pos.y;
					break;
				case RenderWidget::DispMode::DISP_GRID_XZ:
					selPosEnd.x = (int)pos.x;
					selPosEnd.z = (int)pos.z;
					break;
				case RenderWidget::DispMode::DISP_GRID_ZY:
					selPosEnd.z = (int)pos.z;
					selPosEnd.y = (int)pos.y;
					break;
			}
		}

		m_editorState->m_vecSelectionAreaEnd = selPosEnd;
	}

	view->update();
}