// -- Editor4Pane
// Holds the 4-way splitter that contains the 3D view and grids

#include <QDockWidget>
#include <QWidget>

class RenderWidget; // forward declaration
class CChunk;		// forward declaration
class CTool;		// forward declaration

class Editor4Pane : public QDockWidget
{
	Q_OBJECT;

  public:
	Editor4Pane( QWidget *parent );

	void setChunk( CChunk *chunk );
	void setTool( CTool *tool );

  private:
	CChunk *chunk;

	RenderWidget *m_topLeftView;
	RenderWidget *m_bottomLeftView;
	RenderWidget *m_topRightView;
	RenderWidget *m_bottomRightView;
};