// -- Editor4Pane
// Holds the 4-way splitter that contains the 3D view and grids

#include <QDockWidget>
#include <QWidget>

class RenderWidget; // forward declaration
class CChunk;		// forward declaration
class CTool;		// forward declaration
class CWorld;

class Editor4Pane : public QDockWidget
{
	Q_OBJECT;

  public:
	Editor4Pane( QWidget *parent );

	void setChunk( CChunk *chunk );
	void setWorld( CWorld *world );
	void setTool( CTool *tool );

  private:
	// used in "Single Chunk Mode", as the chunk to focus on.
	CChunk *chunk;

	CWorld	*m_world;

	RenderWidget *m_topLeftView;
	RenderWidget *m_bottomLeftView;
	RenderWidget *m_topRightView;
	RenderWidget *m_bottomRightView;
};