// -- Editor4Pane
// Holds the 4-way splitter that contains the 3D view and grids

#include <QWidget>
#include <QDockWidget>

class RenderWidget; // forward declaration
class GridWidget; // forward declaration
class CChunk; // forward declaration

class Editor4Pane : public QDockWidget
{
    Q_OBJECT;

    public:
        Editor4Pane(QWidget *parent);

        void setChunk(CChunk *chunk);

    private:
        CChunk *chunk;

        RenderWidget *m_viewport;
        GridWidget *m_gridXY;
        GridWidget *m_gridXZ;
        GridWidget *m_gridYZ;
};