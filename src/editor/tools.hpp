// -- Tools
// Defines the different tools that can be used in the editor.
// A tool is something that acts when clicked in a viewport.
// Handling of the tool is basically;
// There's a base class tool, and then they derive from it.
// Tools are given the world coordinates of the mouse click,
// A pointer to the calling viewport, and a direction for a raycast.

#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>
#include "world/vector.hpp"

#include "blockdefs.hpp"

class RenderWidget; // forward declaration
class EditorState; // forward declaration

class CTool : public QObject
{
	Q_OBJECT

  public:
	CTool( EditorState *editorState, QObject *parent = 0 );
	virtual ~CTool();

	virtual QString getName() = 0;

	virtual void mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir,
								  RenderWidget *view )	 = 0; // Called when the mouse is pressed
	virtual void mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir,
								 RenderWidget *view )	 = 0; // Called when the mouse is moved
	virtual void mouseReleaseEvent( QMouseEvent *event, Vector3f pos, Vector3f dir,
									RenderWidget *view ) = 0; // Called when the mouse is released
	virtual void wheelEvent( QWheelEvent *event, Vector3f pos, Vector3f dir,
							 RenderWidget *view )		 = 0; // Called when the mouse wheel is used

	virtual void keyPressEvent( QKeyEvent *event, Vector3f pos, Vector3f dir,
								RenderWidget *view )   = 0; // Called when a key is pressed
	virtual void keyReleaseEvent( QKeyEvent *event, Vector3f pos, Vector3f dir,
								  RenderWidget *view ) = 0; // Called when a key is released

	virtual void draw( RenderWidget *view ) = 0; // This is called during the RenderWidget's OpenGL paint event.

	protected:
		EditorState *m_editorState;
};

// Placing/Breaking like minecraft.
class CHandTool final : public CTool
{
	Q_OBJECT

  public:
	CHandTool( EditorState *editorState, QObject *parent = 0 );
	~CHandTool();

	QString getName() { return "Hand"; };

	void mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view );
	void mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view );
	void wheelEvent( QWheelEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view );
	void draw( RenderWidget *view );

	void mouseReleaseEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};
	void keyPressEvent( QKeyEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};
	void keyReleaseEvent( QKeyEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};

  private:
	Vector3f m_selectedBlockPos;
	Vector3f m_selectedBlockNormal;
};

// Editing the properties of a given block.
class CWrenchTool final : public CTool
{
	Q_OBJECT

  public:
	CWrenchTool( EditorState *editorState, QObject *parent = 0 );
	~CWrenchTool();

	QString getName() { return "Wrench"; };

	void mousePressEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view );
	void mouseMoveEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view );
	void draw( RenderWidget *view );

	void mouseReleaseEvent( QMouseEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};
	void keyPressEvent( QKeyEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};
	void keyReleaseEvent( QKeyEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};
	void wheelEvent( QWheelEvent *event, Vector3f pos, Vector3f dir, RenderWidget *view ){};

  private:
	Vector3f m_selectedBlockPos;
	Vector3f m_selectedBlockNormal;
};