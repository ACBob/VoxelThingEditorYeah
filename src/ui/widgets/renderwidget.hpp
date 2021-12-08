// -- Render Widget
// all da opengl stuff

#include "world/vector.hpp"
#include <QGLWidget>
#include <QMenu>
#include <QOpenGLTexture>

class CChunk;	// forward declaration
class CRaycast; // forward declaration
class CTool;	// forward declaration
class CWorld;
class EditorState;

class RenderWidget : public QGLWidget
{
	Q_OBJECT;

  public:
	RenderWidget( EditorState *m_editorState, QWidget *parent = 0 );

	void initializeGL();
	void paintGL();
	void resizeGL( int width, int height );

	void setChunk( CChunk *chunk );
	void setWorld( CWorld *world );

	void keyPressEvent( QKeyEvent *event );
	void mouseMoveEvent( QMouseEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void wheelEvent( QWheelEvent *event );

	enum class DispMode : int {
		DISP_3D		   = 0, // Displays in 3D
		DISP_GRID_XY   = 1, // Draws a grid in the XY plane
		DISP_GRID_XZ   = 2, // Draws a grid in the XZ plane
		DISP_GRID_ZY   = 3, // Draws a grid in the ZY plane
		DISP_ISOMETRIC = 4, // Draws an isometric view
	};

	void setDispMode( DispMode mode );

	Vector3f m_camera;
	Vector3f m_camera_forward;
	Vector3f m_camera_right;

  public slots:
	void setView( QAction *action );

  private:
	QMenu *m_viewDropdown;

	float m_camera_pitch;
	float m_camera_yaw;

	float m_zoom; // Used for grid & isometric

	float offset_x; // Used for grid & isometric
	float offset_y; // Used for grid & isometric

	QPoint m_lastMousePos;
	bool m_captureMouse;
	DispMode m_displayMode;
	QOpenGLTexture *m_texture;
	CRaycast *m_raycast;

	EditorState *m_editorState;

	// stores the GL stuff
	double m_modelview[16];
	double m_projection[16];
	int m_viewport[4];
};