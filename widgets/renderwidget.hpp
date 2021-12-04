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

    private:
        CChunk *m_chunk;

        QVector3D m_camera;
        QVector3D m_camera_target;
};