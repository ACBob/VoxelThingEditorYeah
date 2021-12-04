// -- Render Widget
// all da opengl stuff

#include <QGLWidget>
#include <QVector3D>

class CChunk; // forward declaration

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

    private:
        CChunk *m_chunk;

        QVector3D m_camera;
        QVector3D m_camera_forward;
        QVector3D m_camera_right;

        float m_camera_pitch;
        float m_camera_yaw;

        QPoint m_lastMousePos;
};