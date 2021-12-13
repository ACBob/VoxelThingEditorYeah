#include "renderwidget.hpp"

#include <GL/glu.h>

#include <QOpenGLFunctions_3_3_Core>

#include "editor/editorstate.hpp"
#include "editor/tools.hpp"
#include "world/chunk.hpp"
#include "world/raycast.hpp"
#include "world/world.hpp"

#include "world/vector.hpp"
#include <QDebug>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMenu>

#include <QSettings>

#include <math.h>

RenderWidget::RenderWidget( EditorState *editorState, QWidget *parent ) : QGLWidget( parent )
{
	setFocusPolicy( Qt::StrongFocus );
	setMouseTracking( true );

	m_editorState = editorState;

	m_camera_pitch	 = 0.0f;
	m_camera_yaw	 = 0.0f;
	m_camera_forward = Vector3f( 0.0f, 0.0f, -1.0f );
	m_camera_right	 = Vector3f( 1.0f, 0.0f, 0.0f );
	m_camera		 = Vector3f( 0.0f, 0.0f, 0.0f );

	m_captureMouse = false;

	m_raycast = new CRaycast( this );

	m_zoom	 = 12.0f;
	offset_x = -8.0f;
	offset_y = -8.0f;

	m_viewDropdown = new QMenu( this );
	m_viewDropdown->addAction( "3D" );
	m_viewDropdown->addAction( "XY" );
	m_viewDropdown->addAction( "XZ" );
	m_viewDropdown->addAction( "ZY" );
	m_viewDropdown->addAction( "Isometric" );

	m_viewDropdown->setActiveAction( m_viewDropdown->actions().at( 0 ) );

	connect( m_viewDropdown, SIGNAL( triggered( QAction * ) ), this, SLOT( setView( QAction * ) ) );

	memset( m_modelview, 0, sizeof( m_modelview ) );
	memset( m_projection, 0, sizeof( m_projection ) );
	memset( m_viewport, 0, sizeof( m_viewport ) );

	// setMinimumSize( 320, 240 );
}

void RenderWidget::initializeGL()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	glBlendEquation( GL_FUNC_ADD );

	m_texture = new QOpenGLTexture( QImage( m_editorState->m_sBlockTexturePath ).mirrored() );
	m_texture->setMinificationFilter( QOpenGLTexture::LinearMipMapLinear );
	m_texture->setMagnificationFilter( QOpenGLTexture::Linear );
	m_texture->setWrapMode( QOpenGLTexture::Repeat );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
}

void RenderWidget::resizeGL( int w, int h ) { glViewport( 0, 0, w, h ); }

void RenderWidget::paintGL()
{
	QSettings settings;

	QColor voidColor = settings.value( "voidColor", QColor( Qt::black ) ).value<QColor>();
	QColor gridColor = settings.value( "gridColor", QColor( Qt::gray ) ).value<QColor>();

	glClearColor( voidColor.redF(), voidColor.greenF(), voidColor.blueF(), 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if ( m_editorState->m_pWorld != nullptr && m_displayMode == DispMode::DISP_3D )
	{
		// first-person camera
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 70.0f, (float)width() / (float)height(), 0.1f, 100.0f );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		Vector3f target = m_camera + m_camera_forward;
		gluLookAt( m_camera.x, m_camera.y, m_camera.z, target.x, target.y, target.z, 0.0f, 1.0f, 0.0f );

		// Draw an axis helper
		glBegin( GL_LINES );
		glLineWidth( 4.0f );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 16.0f, 0.0f, 0.0f );
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 16.0f, 0.0f );
		glColor3f( 0.0f, 0.0f, 1.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 16.0f );
		glEnd();
		glLineWidth( 1.0f );

		m_texture->bind();

		m_editorState->m_pWorld->render( context() );

		m_texture->release();
	}
	else if ( m_editorState->m_pWorld != nullptr && m_displayMode >= DispMode::DISP_GRID_XY &&
			  m_displayMode <= DispMode::DISP_GRID_ZY )
	{
		// orthographic camera
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		// setup zoomed viewport dimensions that also care about the aspect ratio
		float w			 = (float)width();
		float h			 = (float)height();
		float aspect	 = w / h;
		float viewport_w = m_zoom;
		float viewport_h = m_zoom;
		if ( aspect > 1.0f )
		{
			viewport_w = m_zoom * aspect;
		}
		else
		{
			viewport_h = m_zoom / aspect;
		}

		glOrtho( -viewport_w / 2.0f, viewport_w / 2.0f, -viewport_h / 2.0f, viewport_h / 2.0f, -100.0f, 100.0f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		if ( m_displayMode == DispMode::DISP_GRID_XY )
		{
			// Looking towards negative Z
			gluLookAt( 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
			glTranslatef( offset_x, offset_y, 0.0f );
		}
		else if ( m_displayMode == DispMode::DISP_GRID_XZ )
		{
			// Looking towards negative Y
			gluLookAt( 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
			glTranslatef( offset_x, 0.0f, offset_y );
		}
		else
		{
			// Looking towards negative X
			gluLookAt( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
			glTranslatef( 0.0f, offset_y, offset_x );
		}

		// After that is setup, draw!
		m_texture->bind();

		m_editorState->m_pWorld->render( context() );

		m_texture->release();

		// draw a 3d grid
		// Wasteful, but I can't be bothered to do it properly
		glDisable( GL_DEPTH_TEST );
		glBegin( GL_LINES );
		glLineWidth( 1.0f );
		glColor3f( gridColor.redF(), gridColor.greenF(), gridColor.blueF() );

		// the grid is rendered snapped to world coordinates
		// but it follows the camera
		float grid_spacing	= 1.0f;
		float grid_offset_x = -offset_x - ( viewport_w / 2.0f );
		float grid_offset_y = -offset_y - ( viewport_h / 2.0f );

		float grid_begin_x = grid_offset_x - ( grid_offset_x - (int)grid_offset_x ) / grid_spacing * grid_spacing;
		float grid_begin_y = grid_offset_y - ( grid_offset_y - (int)grid_offset_y ) / grid_spacing * grid_spacing;
		float grid_end_x   = grid_begin_x + (int)( viewport_w / grid_spacing ) * grid_spacing;
		float grid_end_y   = grid_begin_y + (int)( viewport_h / grid_spacing ) * grid_spacing;

		// Add some extra padding to the grid, so there's no clipping at the edges of the viewport
		grid_begin_x -= grid_spacing * 2.0f;
		grid_begin_y -= grid_spacing * 2.0f;
		grid_end_x += grid_spacing * 2.0f;
		grid_end_y += grid_spacing * 2.0f;

		// XY plane
		if ( m_displayMode == DispMode::DISP_GRID_XY )
		{
			for ( float x = grid_begin_x; x <= grid_end_x; x += grid_spacing )
			{
				glVertex3f( x, grid_begin_y, 0.0f );
				glVertex3f( x, grid_end_y, 0.0f );
			}
			for ( float y = grid_begin_y; y <= grid_end_y; y += grid_spacing )
			{
				glVertex3f( grid_begin_x, y, 0.0f );
				glVertex3f( grid_end_x, y, 0.0f );
			}
		}
		// XZ plane
		else if ( m_displayMode == DispMode::DISP_GRID_XZ )
		{
			for ( float x = grid_begin_x; x <= grid_end_x; x += grid_spacing )
			{
				glVertex3f( x, 0.0f, grid_begin_y );
				glVertex3f( x, 0.0f, grid_end_y );
			}
			for ( float y = grid_begin_y; y <= grid_end_y; y += grid_spacing )
			{
				glVertex3f( grid_begin_x, 0.0f, y );
				glVertex3f( grid_end_x, 0.0f, y );
			}
		}
		// ZY plane
		else if ( m_displayMode == DispMode::DISP_GRID_ZY )
		{
			for ( float x = grid_begin_x; x <= grid_end_x; x += grid_spacing )
			{
				glVertex3f( 0.0f, grid_begin_y, x );
				glVertex3f( 0.0f, grid_end_y, x );
			}
			for ( float y = grid_begin_y; y <= grid_end_y; y += grid_spacing )
			{
				glVertex3f( 0.0f, y, grid_begin_x );
				glVertex3f( 0.0f, y, grid_end_x );
			}
		}

		glEnd();

		glEnable( GL_DEPTH_TEST );
	}
	else if ( m_editorState->m_pWorld != nullptr && m_displayMode == DispMode::DISP_ISOMETRIC )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		// setup zoomed viewport dimensions that also care about the aspect ratio
		float w			 = (float)width();
		float h			 = (float)height();
		float aspect	 = w / h;
		float viewport_w = m_zoom;
		float viewport_h = m_zoom;
		if ( aspect > 1.0f )
		{
			viewport_w = m_zoom * aspect;
		}
		else
		{
			viewport_h = m_zoom / aspect;
		}

		glOrtho( -viewport_w / 2.0f, viewport_w / 2.0f, -viewport_h / 2.0f, viewport_h / 2.0f, -100.0f, 100.0f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		float d = sqrt( 1 / 3.0f );
		gluLookAt( d, d, d, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

		// we want offset_x and offset_y to be diagonal
		float realOffsetX = offset_x * sqrt( 3.0f );
		float realOffsetY = offset_y * sqrt( 3.0f );

		glTranslatef( realOffsetX, realOffsetY, 0.0f );

		// same as DISP_3D now
		m_texture->bind();

		m_editorState->m_pWorld->render( context() );

		m_texture->release();
	}

	glGetDoublev( GL_MODELVIEW_MATRIX, m_modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, m_projection );
	glGetIntegerv( GL_VIEWPORT, m_viewport );

	// Draw the selection if the size is not 0
	if ( m_editorState->m_vecSelectionAreaStart != m_editorState->m_vecSelectionAreaEnd )
	{
		// Draw it as a yellow cuboid
		glColor3f( 1.0f, 1.0f, 0.0f );

		if ( m_displayMode >= DispMode::DISP_GRID_XY && m_displayMode <= DispMode::DISP_GRID_ZY )
			glDisable( GL_DEPTH_TEST );

		glBegin( GL_LINES );

		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );
		
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );

		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaStart.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaStart.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaStart.z );

		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaStart.y, m_editorState->m_vecSelectionAreaEnd.z );
		glVertex3f( m_editorState->m_vecSelectionAreaEnd.x, m_editorState->m_vecSelectionAreaEnd.y, m_editorState->m_vecSelectionAreaEnd.z );

		glEnd();

		if ( m_displayMode >= DispMode::DISP_GRID_XY && m_displayMode <= DispMode::DISP_GRID_ZY )
			glEnable( GL_DEPTH_TEST );
	}

	// Draw the tool last
	if ( m_editorState->m_pTool != nullptr )
	{
		m_editorState->m_pTool->draw( this );
	}

	glDisable( GL_DEPTH_TEST );

	// Up in the corner display the mode
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, width(), 0.0, height(), -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// If capture mouse, display a crosshair
	if ( m_captureMouse )
	{
		glColor3f( 1.0f, 1.0f, 1.0f );
		glBegin( GL_LINES );

		float x = width() / 2.0f;
		float y = height() / 2.0f;

		glVertex2f( x - 10.0f, y );
		glVertex2f( x + 10.0f, y );

		glVertex2f( x, y - 10.0f );
		glVertex2f( x, y + 10.0f );

		glEnd();
	}

	// draw a small box
	glColor3f( 0.0f, 0.0f, 0.0f );
	glBegin( GL_QUADS );
	glVertex2f( 0.0f, height() );
	glVertex2f( 0.0f, height() - 20.0f );
	glVertex2f( 100.0f, height() - 20.0f );
	glVertex2f( 100.0f, height() );
	glEnd();

	QString mode;
	switch ( m_displayMode )
	{
		case DispMode::DISP_3D:
			mode = "Normal";
			break;
		case DispMode::DISP_GRID_XY:
			mode = "Front (X/Y)";
			break;
		case DispMode::DISP_GRID_XZ:
			mode = "Top (X/Z)";
			break;
		case DispMode::DISP_GRID_ZY:
			mode = "Side (Y/Z)";
			break;
		case DispMode::DISP_ISOMETRIC:
			mode = "Isometric";
			break;
	}
	glColor3f( 1.0f, 1.0f, 1.0f );
	renderText( 10, 15, mode );

	glEnable( GL_DEPTH_TEST );
}

void RenderWidget::keyPressEvent( QKeyEvent *event )
{
	switch ( event->key() )
	{
		case Qt::Key_W:
			m_camera += m_camera_forward;
			break;
		case Qt::Key_S:
			m_camera -= m_camera_forward;
			break;
		case Qt::Key_A:
			m_camera -= m_camera_right;
			break;
		case Qt::Key_D:
			m_camera += m_camera_right;
			break;
		case Qt::Key_Q:
			m_camera -= Vector3f( 0.0f, 1.0f, 0.0f );
			break;
		case Qt::Key_E:
			m_camera += Vector3f( 0.0f, 1.0f, 0.0f );
			break;
		case Qt::Key_Z:
			m_captureMouse = !m_captureMouse && m_displayMode == DispMode::DISP_3D;
			if ( m_captureMouse )
				setCursor( Qt::BlankCursor );
			else
				unsetCursor();
			break;
	}

	update();
}

void RenderWidget::mouseMoveEvent( QMouseEvent *event )
{
	if ( m_captureMouse )
	{
		// camera_forward is -1.0f on Z, rotated by the pitch and yaw
		m_camera_yaw += ( event->x() - m_lastMousePos.x() ) * 0.1f;
		m_camera_pitch += ( event->y() - m_lastMousePos.y() ) * 0.1f;
		m_camera_pitch = qBound( -89.0f, m_camera_pitch, 89.0f );

		m_camera_forward =
			Vector3f( 0.0f, 0.0f, 1.0f ).Rotate( Vector3f( m_camera_pitch, -m_camera_yaw, 0.0f ) ).Normal();
		m_camera_right =
			Vector3f( 1.0f, 0.0f, 0.0f ).Rotate( Vector3f( m_camera_pitch, -m_camera_yaw, 0.0f ) ).Normal();

		m_camera_right = m_camera_forward.Cross( Vector3f( 0.0f, 1.0f, 0.0f ) );

		update();
	}

	if ( m_displayMode >= DispMode::DISP_GRID_XY && m_displayMode <= DispMode::DISP_ISOMETRIC &&
		 event->buttons() & Qt::MiddleButton )
	{
		offset_x += ( event->x() - m_lastMousePos.x() ) * 0.01f;
		offset_y += ( event->y() - m_lastMousePos.y() ) * 0.01f;
		update();

		// Move mouse back to where it was
		QCursor::setPos( mapToGlobal( m_lastMousePos ) );
		m_lastMousePos = mapFromGlobal( QCursor::pos() );

		// make the cursor look like it's dragging
		setCursor( Qt::ClosedHandCursor );

		// qDebug() << "offset_x: " << offset_x << " offset_y: " << offset_y;
	}
	else
	{
		m_lastMousePos = event->pos();
		unsetCursor();
	}

	// If mouse is captured, we want to reset the cursor position
	if ( m_captureMouse )
	{
		QCursor::setPos( mapToGlobal( QPoint( width() / 2, height() / 2 ) ) );
		m_lastMousePos = QPoint( width() / 2, height() / 2 );
	}

	GLdouble x, y, z;
	gluUnProject( event->x(), height() - event->y(), 0.0f, m_modelview, m_projection, m_viewport, &x, &y, &z );

	Vector3f ray_direction;
	if ( m_displayMode == DispMode::DISP_3D )
		ray_direction = Vector3f( x, y, z ) - m_camera;
	else if ( m_displayMode == DispMode::DISP_GRID_XY )
		ray_direction = Vector3f( 0, 0, -1 );
	else if ( m_displayMode == DispMode::DISP_GRID_XZ )
		ray_direction = Vector3f( 0, -1, 0 );
	else if ( m_displayMode == DispMode::DISP_GRID_ZY )
		ray_direction = Vector3f( -1, 0, 0 );
	else if ( m_displayMode == DispMode::DISP_ISOMETRIC )
		ray_direction = Vector3f( x, y, z ) - Vector3f( m_modelview[12], m_modelview[13], m_modelview[14] );
	ray_direction = ray_direction.Normal();

	m_editorState->m_pTool->mouseMoveEvent( event, Vector3f( x, y, z ), ray_direction, this );
}

void RenderWidget::mousePressEvent( QMouseEvent *event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		// test if it's within the little mode marker
		if ( event->x() < 100 && event->y() < 20 )
		{
			// int mode = (int)m_displayMode;
			// mode++;
			// if (mode > (int)DispMode::DISP_ISOMETRIC)
			//     mode = (int)DispMode::DISP_3D;
			// m_displayMode = (DispMode)mode;

			// If it is, display a dropdown menu
			m_viewDropdown->move( event->globalPos() );
			m_viewDropdown->show();
			update();
			return;
		}
	}

	if ( m_editorState->m_pTool != nullptr )
	{
		if ( !m_captureMouse )
		{
			// Unproject the mouse position to get the world position
			// gluUnproject is used
			GLdouble x, y, z;
			gluUnProject( event->x(), height() - event->y(), 0.0f, m_modelview, m_projection, m_viewport, &x, &y, &z );

			// we then need to figure out the direction of the ray
			Vector3f ray_direction;
			if ( m_displayMode == DispMode::DISP_3D )
				ray_direction = Vector3f( x, y, z ) - m_camera;
			else if ( m_displayMode == DispMode::DISP_GRID_XY )
				ray_direction = Vector3f( 0, 0, -1 );
			else if ( m_displayMode == DispMode::DISP_GRID_XZ )
				ray_direction = Vector3f( 0, -1, 0 );
			else if ( m_displayMode == DispMode::DISP_GRID_ZY )
				ray_direction = Vector3f( -1, 0, 0 );
			else if ( m_displayMode == DispMode::DISP_ISOMETRIC )
				ray_direction = Vector3f( x, y, z ) - Vector3f( m_modelview[12], m_modelview[13], m_modelview[14] );
			ray_direction = ray_direction.Normal();

			m_editorState->m_pTool->mousePressEvent( event, Vector3f( x, y, z ), ray_direction, this );
		}
		else
		{
			m_editorState->m_pTool->mousePressEvent( event, m_camera, m_camera_forward, this );
		}
	}
}

void RenderWidget::setDispMode( DispMode mode )
{
	m_displayMode = mode;
	update();
}

RenderWidget::DispMode RenderWidget::getDispMode() const
{
	return m_displayMode;
}

void RenderWidget::wheelEvent( QWheelEvent *event )
{
	if ( m_displayMode == DispMode::DISP_3D )
	{
		if ( m_editorState->m_pTool != nullptr )
			m_editorState->m_pTool->wheelEvent( event, m_camera, m_camera_forward, this );
		update();
		return;
	}

	m_zoom += event->delta() * 0.001f;
	m_zoom = qBound( 0.1f, m_zoom, 20.0f );
	update();
}

void RenderWidget::setView( QAction *action )
{
	if ( action->text() == "3D" )
		m_displayMode = DispMode::DISP_3D;
	else if ( action->text() == "Isometric" )
		m_displayMode = DispMode::DISP_ISOMETRIC;
	else if ( action->text() == "XY" )
		m_displayMode = DispMode::DISP_GRID_XY;
	else if ( action->text() == "XZ" )
		m_displayMode = DispMode::DISP_GRID_XZ;
	else if ( action->text() == "ZY" )
		m_displayMode = DispMode::DISP_GRID_ZY;

	m_viewDropdown->setActiveAction( action );
}