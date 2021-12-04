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
}

void RenderWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void RenderWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glBegin(GL_TRIANGLES);
    // glColor3f(1.0f, 0.0f, 0.0f);
    // glVertex3f(-0.5f, -0.5f, 0.0f);
    // glColor3f(0.0f, 1.0f, 0.0f);
    // glVertex3f(0.5f, -0.5f, 0.0f);
    // glColor3f(0.0f, 0.0f, 1.0f);
    // glVertex3f(0.0f, 0.5f, 0.0f);
    // glEnd();

    if (m_chunk == nullptr)
        return;
    
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first-person camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    QVector3D target = m_camera + m_camera_forward;
    gluLookAt(m_camera.x(), m_camera.y(), m_camera.z(), target.x(), target.y(), target.z(), 0.0f, 1.0f, 0.0f);

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
                    glVertex3f(x, y, z);
                    glVertex3f(x + 1, y, z);
                    glVertex3f(x + 1, y + 1, z);
                    glVertex3f(x, y + 1, z);
                    glEnd();
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
    }

    update();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // camera_forward is -1.0f on Z, rotated by the pitch and yaw
        m_camera_yaw += (event->x() - m_lastMousePos.x()) * 0.1f;
        m_camera_pitch += (event->y() - m_lastMousePos.y()) * 0.1f;
        m_camera_pitch = qBound(-89.0f, m_camera_pitch, 89.0f);

        m_camera_forward = QVector3D(0.0f, 0.0f, -1.0f);
        QMatrix4x4 rotation;
        rotation.rotate(m_camera_yaw, 0.0f, 1.0f, 0.0f);
        rotation.rotate(m_camera_pitch, 1.0f, 0.0f, 0.0f);
        m_camera_forward = rotation * m_camera_forward;

        m_camera_right = QVector3D::crossProduct(m_camera_forward, QVector3D(0.0f, 1.0f, 0.0f));

        update();
    }
    
    m_lastMousePos = event->pos();
}