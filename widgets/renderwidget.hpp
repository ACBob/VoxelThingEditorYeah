// -- Render Widget
// all da opengl stuff

#include <QGLWidget>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QMenu>

class CChunk; // forward declaration
class CRaycast; // forward declaration
class CTool; // forward declaration

class RenderWidget : public QGLWidget
{
    Q_OBJECT;

    public:
        RenderWidget(QWidget *parent = 0);
        
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

        void setChunk(CChunk *chunk);

        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);


        enum class DispMode : int {
            DISP_3D = 0, // Displays in 3D
            DISP_GRID_XY = 1, // Draws a grid in the XY plane
            DISP_GRID_XZ = 2, // Draws a grid in the XZ plane
            DISP_GRID_ZY = 3, // Draws a grid in the ZY plane
            DISP_ISOMETRIC = 4, // Draws an isometric view
        };

        void setDispMode(DispMode mode);
        void setTool(CTool *tool);

        CChunk *m_chunk;

        QVector3D m_camera;
        QVector3D m_camera_forward;
        QVector3D m_camera_right;

    public slots:
        void setView(QAction* action);

    private:
        QMenu *m_viewDropdown;

        float m_camera_pitch;
        float m_camera_yaw;

        float m_zoom; // Used for grid & isometric

        QPoint m_lastMousePos;

        bool m_captureMouse;

        DispMode m_displayMode;

        QOpenGLTexture *m_texture;

        CRaycast *m_raycast;

        CTool *m_currentTool;
};