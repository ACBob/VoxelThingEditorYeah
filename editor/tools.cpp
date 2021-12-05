#include "tools.hpp"

#include "../widgets/renderwidget.hpp"

#include "../world/raycast.hpp"
#include "../world/chunk.hpp"

#include <QDebug>

#include <QOpenGLFunctions>
#include <GL/glu.h>

CTool::CTool(QObject *parent) :
    QObject(parent)
{
};
CTool::~CTool()
{
};

CHandTool::CHandTool(QObject* parent) :
    CTool(parent)
{
    m_selectedBlockPos = QVector3D(0, 0, 0);
}

CHandTool::~CHandTool()
{

}

void CHandTool::mousePressEvent(QMouseEvent *event, QVector3D pos, QVector3D dir, RenderWidget* view)
{
    qDebug () << "Hand tool pressed @" << pos;
    // m_selectedBlockPos = pos;
    // view->update();

    CRaycast caster;
    std::pair<QVector3D, QVector3D> cast = caster.cast( view->m_chunk, pos, dir, 100.0f);

    m_selectedBlockPos = cast.first;
    m_selectedBlockNormal = cast.second;

    if (m_selectedBlockNormal.length() > 0.0f)
    {
        if (event->button() == Qt::LeftButton)
        {
            QVector3D p = m_selectedBlockPos;
            view->m_chunk->setID(p.x(), p.y(), p.z(), 0);
            view->m_chunk->setMeta(p.x(), p.y(), p.z(), 0);
        }
        else if ( event->button() == Qt::RightButton)
        {
            QVector3D p = m_selectedBlockPos + m_selectedBlockNormal;
            view->m_chunk->setID(p.x(), p.y(), p.z(), 1);
            view->m_chunk->setMeta(p.x(), p.y(), p.z(), 0);
        }
    }

    view->update();
}

void CHandTool::draw(RenderWidget* view)
{
    // qDebug () << "Hand tool draw";
    // if we have a pointed voxel, draw a wireframe cube around it
    if (m_selectedBlockPos != QVector3D(0, 0, 0))
    {
        glBegin(GL_LINES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        glVertex3f(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z());
        glVertex3f(m_selectedBlockPos.x() + 1, m_selectedBlockPos.y(), m_selectedBlockPos.z());

        glVertex3f(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z());
        glVertex3f(m_selectedBlockPos.x(), m_selectedBlockPos.y() + 1, m_selectedBlockPos.z());

        glVertex3f(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z());
        glVertex3f(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z() + 1);

        glEnd();
    }
}