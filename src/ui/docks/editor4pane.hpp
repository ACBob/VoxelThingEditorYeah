// -- Editor4Pane
// Holds the 4-way splitter that contains the 3D view and grids

#include <QDockWidget>
#include <QWidget>

class RenderWidget; // forward declaration
class EditorState;

class Editor4Pane : public QDockWidget
{
	Q_OBJECT;

  public:
	Editor4Pane( EditorState *editorState, QWidget *parent );

  private:
	RenderWidget *m_topLeftView;
	RenderWidget *m_bottomLeftView;
	RenderWidget *m_topRightView;
	RenderWidget *m_bottomRightView;

	EditorState *m_editorState;
};