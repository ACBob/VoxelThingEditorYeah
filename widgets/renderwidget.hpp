// -- Render Widget
// all da opengl stuff

#include <QGLWidget>
#include <QVector3D>
#include <QOpenGLTexture>

class CChunk; // forward declaration
class CRaycast; // forward declaration

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


        enum DispMode {
            DISP_3D, // Displays in 3D
            DISP_GRID_XY, // Draws a grid in the XY plane
            DISP_GRID_XZ, // Draws a grid in the XZ plane
            DISP_GRID_YZ, // Draws a grid in the YZ plane
        };

        void setDispMode(DispMode mode);

    private:
        CChunk *m_chunk;

        QVector3D m_camera;
        QVector3D m_camera_forward;
        QVector3D m_camera_right;

        float m_camera_pitch;
        float m_camera_yaw;

        QPoint m_lastMousePos;

        bool m_captureMouse;

        DispMode m_displayMode;

        QOpenGLTexture *m_texture;

        CRaycast *m_raycast;
};