// -- Tools
// Defines the different tools that can be used in the editor.
// A tool is something that acts when clicked in a viewport.
// Handling of the tool is basically;
// There's a base class tool, and then they derive from it.
// Tools are given the world coordinates of the mouse click,
// And the display mode of the viewport.
// They can then do whatever they want with that information.
// They have access to the current chunk.

#include <QObject>
#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>

class RenderWidget; // forward declaration

class CTool : public QObject
{
    Q_OBJECT

    public:
        CTool(QObject* parent = 0);
        virtual ~CTool();

        virtual QString getName() = 0;

        virtual void mousePressEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) = 0; // Called when the mouse is pressed
        virtual void mouseMoveEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) = 0; // Called when the mouse is moved
        virtual void mouseReleaseEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) = 0; // Called when the mouse is released
        virtual void mouseDoubleClickEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) = 0; // Called when the mouse is double clicked
        virtual void wheelEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) = 0; // Called when the mouse wheel is used

        virtual void keyPressEvent(QKeyEvent* event, QVector3D pos, RenderWidget* view) = 0; // Called when a key is pressed
        virtual void keyReleaseEvent(QKeyEvent* event, QVector3D pos, RenderWidget* view) = 0; // Called when a key is released

        virtual void draw(RenderWidget* view) = 0; // This is called during the RenderWidget's OpenGL paint event.
};

// Placing/Breaking like minecraft.
class CHandTool final : public CTool
{
    Q_OBJECT

    public:
        CHandTool(QObject* parent = 0);
        ~CHandTool();

        QString getName() {
            return "Hand";
        };

        void mousePressEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view);
        void draw(RenderWidget* view);


        void mouseMoveEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) {};
        void mouseReleaseEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) {};
        void mouseDoubleClickEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) {};
        void wheelEvent(QMouseEvent *event, QVector3D pos, RenderWidget* view) {};
        void keyPressEvent(QKeyEvent* event, QVector3D pos, RenderWidget* view) {};
        void keyReleaseEvent(QKeyEvent* event, QVector3D pos, RenderWidget* view) {};

    private:
        QVector3D m_selectedBlockPos;
};