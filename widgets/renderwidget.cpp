#include "renderwidget.hpp"

#include <GL/glu.h>

#include "../world/chunk.hpp"

#include <QVector3D>
#include <QKeyEvent>
#include <QMatrix4x4>

#include <math.h>

RenderWidget::RenderWidget(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_chunk = nullptr;
    m_camera_pitch = 0.0f;
    m_camera_yaw = 0.0f;
    m_camera_forward = QVector3D(0.0f, 0.0f, -1.0f);
    m_camera_right = QVector3D(1.0f, 0.0f, 0.0f);
    m_camera = QVector3D(0.0f, 0.0f, 0.0f);

    m_captureMouse = false;
}

void RenderWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    m_texture = new QOpenGLTexture(QImage(":/img/devoxel.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void RenderWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Up in the corner display the mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width(), 0.0, height(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    QString mode;
    switch (m_displayMode)
    {
        case DISP_3D:
            mode = "Normal";
            break;
        case DISP_GRID_XY:
            mode = "XY";
            break;
        case DISP_GRID_XZ:
            mode = "XZ";
            break;
        case DISP_GRID_YZ:
            mode = "YZ";
            break;
    }
    renderText(10, 10, mode);


    if (m_chunk == nullptr)
        return;
    
    // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_displayMode == DISP_3D)
    {
        // first-person camera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.0f, (float)width() / (float)height(), 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        QVector3D target = m_camera + m_camera_forward;
        gluLookAt(m_camera.x(), m_camera.y(), m_camera.z(), target.x(), target.y(), target.z(), 0.0f, 1.0f, 0.0f);

        m_texture->bind();

        for (int x = 0; x < m_chunk->getSizeX(); x++)
        {
            for (int y = 0; y < m_chunk->getSizeX(); y++)
            {
                for (int z = 0; z < m_chunk->getSizeX(); z++)
                {
                    if (m_chunk->getID(x, y, z) != 0)
                    {
                        glBegin(GL_QUADS);
                        glColor3f(1.0f, 1.0f, 1.0f);

                        glTexCoord2f(0.0f, 0.0f);
                        glVertex3f(x, y, z);
                        glTexCoord2f(1.0f, 0.0f);
                        glVertex3f(x + 1, y, z);
                        glTexCoord2f(1.0f, 1.0f);
                        glVertex3f(x + 1, y + 1, z);
                        glTexCoord2f(0.0f, 1.0f);
                        glVertex3f(x, y + 1, z);
                        glEnd();
                    }
                }
            }
        }

        m_texture->release();
    }
    else if (m_displayMode >= DISP_GRID_XY && m_displayMode <= DISP_GRID_YZ)
    {
        // orthographic camera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, m_chunk->getSizeX(), 0.0f, m_chunk->getSizeY(), 0.0f, m_chunk->getSizeZ());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw the grid
        glBegin(GL_LINES);
        glColor3f(0.25f, 0.25f, 0.25f);
        for (int x = 0; x <= m_chunk->getSizeX(); x++)
        {
            glVertex3f(x, 0.0f, 0.0f);
            glVertex3f(x, m_chunk->getSizeY(), 0.0f);
        }
        for (int y = 0; y <= m_chunk->getSizeY(); y++)
        {
            glVertex3f(0.0f, y, 0.0f);
            glVertex3f(m_chunk->getSizeX(), y, 0.0f);
        }
        for (int z = 0; z <= m_chunk->getSizeZ(); z++)
        {
            glVertex3f(0.0f, 0.0f, z);
            glVertex3f(m_chunk->getSizeX(), 0.0f, z);
        }
        glEnd();


        m_texture->bind();

        for (int x = 0; x < m_chunk->getSizeX(); x++)
        {
            for (int y = 0; y < m_chunk->getSizeX(); y++)
            {
                for (int z = 0; z < m_chunk->getSizeX(); z++)
                {
                    if (m_chunk->getID(x, y, z) != 0)
                    {
                        glBegin(GL_QUADS);
                        glColor3f(1.0f, 1.0f, 1.0f);

                        glTexCoord2f(0.0f, 0.0f);
                        glVertex3f(x, y, z);
                        glTexCoord2f(1.0f, 0.0f);
                        glVertex3f(x + 1, y, z);
                        glTexCoord2f(1.0f, 1.0f);
                        glVertex3f(x + 1, y + 1, z);
                        glTexCoord2f(0.0f, 1.0f);
                        glVertex3f(x, y + 1, z);
                        glEnd();
                    }
                }
            }
        }
    }
}

void RenderWidget::setChunk(CChunk *chunk)
{
    m_chunk = chunk;
    update();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        m_camera += m_camera_forward;
        break;
    case Qt::Key_S:
        m_camera -= m_camera_forward;
        break;
    case Qt::Key_A:
        m_camera -= m_camera_right;
        break;
    case Qt::Key_D:
        m_camera += m_camera_right;
        break;
    case Qt::Key_Q:
        m_camera -= QVector3D(0.0f, 1.0f, 0.0f);
        break;
    case Qt::Key_E:
        m_camera += QVector3D(0.0f, 1.0f, 0.0f);
        break;
    case Qt::Key_Z:
        m_captureMouse = !m_captureMouse && m_displayMode == DISP_3D;
        if (m_captureMouse)
            setCursor(Qt::BlankCursor);
        else
            unsetCursor();
        break;
    }

    update();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton || m_captureMouse)
    {
        // camera_forward is -1.0f on Z, rotated by the pitch and yaw
        m_camera_yaw += (event->x() - m_lastMousePos.x()) * 0.1f;
        m_camera_pitch += (event->y() - m_lastMousePos.y()) * 0.1f;
        m_camera_pitch = qBound(-89.0f, m_camera_pitch, 89.0f);

        m_camera_forward = QVector3D(0.0f, 0.0f, 1.0f);
        QMatrix4x4 rotation;
        rotation.rotate(-m_camera_yaw, 0.0f, 1.0f, 0.0f);
        rotation.rotate(m_camera_pitch, 1.0f, 0.0f, 0.0f);
        m_camera_forward = rotation * m_camera_forward;

        m_camera_right = QVector3D::crossProduct(m_camera_forward, QVector3D(0.0f, 1.0f, 0.0f));

        update();
    }
    
    m_lastMousePos = event->pos();

    // If mouse is captured, we want to reset the cursor position
    if (m_captureMouse)
    {
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        m_lastMousePos = QPoint(width() / 2, height() / 2);
    }
}

void RenderWidget::setDispMode(DispMode mode)
{
    m_displayMode = mode;
    update();
}