#include "renderwidget.hpp"

#include <GL/glu.h>

#include "../world/chunk.hpp"
#include "../world/raycast.hpp"

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

    m_raycast = new CRaycast(this);

    m_zoom = 1.0f;

    setMinimumSize(320, 240);
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
    
    // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_chunk != nullptr && m_displayMode == DispMode::DISP_3D)
    {
        // first-person camera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.0f, (float)width() / (float)height(), 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        QVector3D target = m_camera + m_camera_forward;
        gluLookAt(m_camera.x(), m_camera.y(), m_camera.z(), target.x(), target.y(), target.z(), 0.0f, 1.0f, 0.0f);

        // Draw an axis helper
        glBegin(GL_LINES);
        glLineWidth(4.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(16.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 16.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 16.0f);
        glEnd();
        glLineWidth(1.0f);

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
    else if (m_chunk != nullptr && m_displayMode >= DispMode::DISP_GRID_XY && m_displayMode <= DispMode::DISP_GRID_ZY)
    {
        // orthographic camera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (m_displayMode == DispMode::DISP_GRID_XY)
        {
            glOrtho( 0.0f, m_chunk->getSizeX(), 0.0f, m_chunk->getSizeY(), 0.0f, 100.0f);
        }
        else if (m_displayMode == DispMode::DISP_GRID_ZY)
        {
            glOrtho( 0.0f, m_chunk->getSizeX(), 0.0f, m_chunk->getSizeZ(), 0.0f, 100.0f);
        }
        else if (m_displayMode == DispMode::DISP_GRID_XZ)
        {
            glOrtho( 0.0f, m_chunk->getSizeY(), 0.0f, m_chunk->getSizeZ(), 0.0f, 100.0f);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // After that is setup, draw!
        if (m_displayMode == DispMode::DISP_GRID_XY)
        {
            // draw the grid
            glBegin(GL_LINES);
            glLineWidth(1.0f);
            glColor3f(0.5f, 0.5f, 0.5f);
            for (int x = 0; x <= m_chunk->getSizeX(); x++)
            {
                glVertex2f(x, 0.0f);
                glVertex2f(x, m_chunk->getSizeY());
            }
            for (int y = 0; y <= m_chunk->getSizeY(); y++)
            {
                glVertex2f(0.0f, y);
                glVertex2f(m_chunk->getSizeX(), y);
            }
            glEnd();

            // draw the blocks
            m_texture->bind();

            for ( int z = 0; z < m_chunk->getSizeZ(); z++ )
            {
                float color = (float)z / (float)m_chunk->getSizeZ();
                color = 1.0f - color;
                glColor3f(color, color, color);
                for (int x = 0; x < m_chunk->getSizeX(); x++)
                {
                    for (int y = 0; y < m_chunk->getSizeZ(); y++)
                    {
                        if (m_chunk->getID(x, y, z) != 0)
                        {
                            glBegin(GL_QUADS);
                            glTexCoord2f(0.0f, 0.0f);
                            glVertex2f(x, y);
                            glTexCoord2f(1.0f, 0.0f);
                            glVertex2f(x + 1, y);
                            glTexCoord2f(1.0f, 1.0f);
                            glVertex2f(x + 1, y + 1);
                            glTexCoord2f(0.0f, 1.0f);
                            glVertex2f(x, y + 1);
                            glEnd();
                        }
                    }
                }
            }

            m_texture->release();
        }
        else if (m_displayMode == DispMode::DISP_GRID_ZY)
        {
            // draw the grid
            glBegin(GL_LINES);
            glLineWidth(1.0f);
            glColor3f(0.5f, 0.5f, 0.5f);
            for (int z = 0; z <= m_chunk->getSizeZ(); z++)
            {
                glVertex2f(z, 0.0f);
                glVertex2f(z, m_chunk->getSizeY());
            }
            for (int y = 0; y <= m_chunk->getSizeY(); y++)
            {
                glVertex2f(0.0f, y);
                glVertex2f(m_chunk->getSizeZ(), y);
            }
            glEnd();

            // draw the blocks
            m_texture->bind();

            for (int x = 0; x < m_chunk->getSizeX(); x++)
            {
                float color = (float)x / (float)m_chunk->getSizeX();
                color = 1.0f - color;
                glColor3f(color, color, color);
                for (int z = 0; z < m_chunk->getSizeZ(); z++)
                {
                    for (int y = 0; y < m_chunk->getSizeY(); y++)
                    {
                        if (m_chunk->getID(x, y, z) != 0)
                        {
                            glBegin(GL_QUADS);
                            glTexCoord2f(0.0f, 0.0f);
                            glVertex2f(z, y);
                            glTexCoord2f(1.0f, 0.0f);
                            glVertex2f(z + 1, y);
                            glTexCoord2f(1.0f, 1.0f);
                            glVertex2f(z + 1, y + 1);
                            glTexCoord2f(0.0f, 1.0f);
                            glVertex2f(z, y + 1);
                            glEnd();
                        }
                    }
                }
            }
            
            m_texture->release();
        }
        else if (m_displayMode == DispMode::DISP_GRID_XZ)
        {
            // draw the grid
            glBegin(GL_LINES);
            glLineWidth(1.0f);
            glColor3f(0.5f, 0.5f, 0.5f);
            for (int x = 0; x <= m_chunk->getSizeY(); x++)
            {
                glVertex2f(x, 0.0f);
                glVertex2f(x, m_chunk->getSizeZ());
            }
            for (int y = 0; y <= m_chunk->getSizeZ(); y++)
            {
                glVertex2f(0.0f, y);
                glVertex2f(m_chunk->getSizeY(), y);
            }
            glEnd();

            // draw the blocks
            m_texture->bind();

            for ( int y = 0; y < m_chunk->getSizeY(); y++ )
            {
                float color = (float)y / (float)m_chunk->getSizeY();
                color = 1.0f - color;
                glColor3f(color, color, color);
                for (int x = 0; x < m_chunk->getSizeX(); x++)
                {
                    for (int z = 0; z < m_chunk->getSizeZ(); z++)
                    {
                        if (m_chunk->getID(x, y, z) != 0)
                        {
                            glBegin(GL_QUADS);
                            glTexCoord2f(0.0f, 0.0f);
                            glVertex2f(x, z);
                            glTexCoord2f(1.0f, 0.0f);
                            glVertex2f(x + 1, z);
                            glTexCoord2f(1.0f, 1.0f);
                            glVertex2f(x + 1, z + 1);
                            glTexCoord2f(0.0f, 1.0f);
                            glVertex2f(x, z + 1);
                            glEnd();
                        }
                    }
                }
            }

            m_texture->release();
        }
    }
    else if (m_chunk != nullptr && m_displayMode == DispMode::DISP_ISOMETRIC)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        glOrtho( -m_zoom, m_zoom, -m_zoom, m_zoom, -100.0f, 100.0f );

        float d = sqrt( 1 / 3.0f );
        gluLookAt(d, d, d, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // same as DISP_3D now
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

    glDisable(GL_DEPTH_TEST);

    // Up in the corner display the mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width(), 0.0, height(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw a small box
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f( 0.0f, height() );
    glVertex2f( 0.0f, height() - 20.0f );
    glVertex2f( 100.0f, height() - 20.0f );
    glVertex2f( 100.0f, height() );
    glEnd();

    QString mode;
    switch (m_displayMode)
    {
        case DispMode::DISP_3D:
            mode = "Normal";
            break;
        case DispMode::DISP_GRID_XY:
            mode = "XY";
            break;
        case DispMode::DISP_GRID_XZ:
            mode = "XZ";
            break;
        case DispMode::DISP_GRID_ZY:
            mode = "YZ";
            break;
        case DispMode::DISP_ISOMETRIC:
            mode = "Isometric";
            break;
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText(10, 15, mode);

    glEnable(GL_DEPTH_TEST);
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
        m_captureMouse = !m_captureMouse && m_displayMode == DispMode::DISP_3D;
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
    if (event->buttons() & Qt::MidButton || m_captureMouse)
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

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // test if it's within the little mode marker
        if (event->x() < 100 && event->y() < 20)
        {
            int mode = (int)m_displayMode;
            mode++;
            if (mode > (int)DispMode::DISP_ISOMETRIC)
                mode = (int)DispMode::DISP_3D;
            m_displayMode = (DispMode)mode;
        }

        update();
    }
}

void RenderWidget::setDispMode(DispMode mode)
{
    m_displayMode = mode;
    update();
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
    m_zoom += event->delta() * 0.001f;
    m_zoom = qBound(0.1f, m_zoom, 20.0f);
    update();
}