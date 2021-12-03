// -- Editor4Pane
// Holds the 4-way splitter that contains the 3D view and grids

#include <QWidget>
#include <QDockWidget>

class Editor4Pane : public QDockWidget
{
    Q_OBJECT;

    public:
        Editor4Pane(QWidget *parent);
};