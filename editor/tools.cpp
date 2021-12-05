#include "tools.hpp"

#include "../widgets/renderwidget.hpp"

#include "../world/raycast.hpp"
#include "../world/chunk.hpp"

#include <QDebug>

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>

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

    view->m_chunk->rebuildModel();
    
    view->update();
}

void CHandTool::mouseMoveEvent(QMouseEvent *event, QVector3D pos, QVector3D dir, RenderWidget* view)
{
    // qDebug () << "Hand tool moved @" << pos;
    
    CRaycast caster;
    std::pair<QVector3D, QVector3D> cast = caster.cast( view->m_chunk, pos, dir, 100.0f);

    m_selectedBlockPos = cast.first;
    m_selectedBlockNormal = cast.second;

    view->update();
}

const static QVector3D cubeLinesVertices[32] = {
    // Bottom
    QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
    QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
    QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 1.0f),
    QVector3D(0.0f, 0.0f, 1.0f), QVector3D(1.0f, 0.0f, 1.0f),

    // Top
    QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 1.0f, 0.0f),
    QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 1.0f, 1.0f),
    QVector3D(1.0f, 1.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f),
    QVector3D(0.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f, 1.0f),

    // Sides
    QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f),
    QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 0.0f),

    QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 1.0f),
    QVector3D(1.0f, 0.0f, 1.0f), QVector3D(1.0f, 1.0f, 1.0f),

    QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
    QVector3D(1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 1.0f),

    QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 1.0f, 1.0f),
    QVector3D(1.0f, 1.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f),
};

void CHandTool::draw(RenderWidget* view)
{
    // qDebug () << "Hand tool draw";
    // if we have a pointed voxel, draw a wireframe cube around it
    if (m_selectedBlockPos != QVector3D(0, 0, 0))
    {
        glBegin(GL_LINES);
        glLineWidth(15.0f);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);

        for (int i = 0; i < 32; i += 2)
        {
            QVector3D p1 = m_selectedBlockPos + cubeLinesVertices[i];
            QVector3D p2 = m_selectedBlockPos + cubeLinesVertices[i + 1];

            glVertex3f(p1.x(), p1.y(), p1.z());
            glVertex3f(p2.x(), p2.y(), p2.z());
        }

        glEnd();
    }
}


// Wrench
CWrenchTool::CWrenchTool(QObject* parent) :
    CTool(parent)
{
}

CWrenchTool::~CWrenchTool()
{

}

void CWrenchTool::mousePressEvent(QMouseEvent *event, QVector3D pos, QVector3D dir, RenderWidget* view)
{
    qDebug () << "Wrench tool pressed @" << pos;
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
            if (m_selectedBlockNormal != QVector3D(0, 0, 0))
            {
                // Display configure dialog
                QDialog* dlg = new QDialog(view);
                dlg->setWindowTitle("Configure block");
                
                QFormLayout* layout = new QFormLayout(dlg);
                QLineEdit* idInp = new QLineEdit(dlg);
                idInp->setText(QString::number(view->m_chunk->getID(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z())));
                QLineEdit* metaInp = new QLineEdit(dlg);
                metaInp->setText(QString::number(view->m_chunk->getMeta(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z())));
                
                layout->addRow("ID", idInp);
                layout->addRow("Meta", metaInp);

                QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dlg);
                layout->addRow(buttons);

                connect(buttons, SIGNAL(accepted()), dlg, SLOT(accept()));
                connect(buttons, SIGNAL(rejected()), dlg, SLOT(reject()));

                if (dlg->exec() == QDialog::Accepted)
                {
                    int id = idInp->text().toInt();
                    int meta = metaInp->text().toInt();

                    view->m_chunk->setID(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z(), id);
                    view->m_chunk->setMeta(m_selectedBlockPos.x(), m_selectedBlockPos.y(), m_selectedBlockPos.z(), meta);
                    view->m_chunk->rebuildModel();
                    view->update();
                }
            }
        }
    }

    view->m_chunk->rebuildModel();
    
    view->update();
}

void CWrenchTool::mouseMoveEvent(QMouseEvent *event, QVector3D pos, QVector3D dir, RenderWidget* view)
{
    // qDebug () << "Wrench tool moved @" << pos;
    
    CRaycast caster;
    std::pair<QVector3D, QVector3D> cast = caster.cast( view->m_chunk, pos, dir, 100.0f);

    m_selectedBlockPos = cast.first;
    m_selectedBlockNormal = cast.second;

    view->update();
}

void CWrenchTool::draw(RenderWidget* view)
{
    // qDebug () << "Wrench tool draw";
    // if we have a pointed voxel, draw a wireframe cube around it
    if (m_selectedBlockPos != QVector3D(0, 0, 0))
    {
        glBegin(GL_LINES);
        glLineWidth(15.0f);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);

        for (int i = 0; i < 32; i += 2)
        {
            QVector3D p1 = m_selectedBlockPos + cubeLinesVertices[i];
            QVector3D p2 = m_selectedBlockPos + cubeLinesVertices[i + 1];

            glVertex3f(p1.x(), p1.y(), p1.z());
            glVertex3f(p2.x(), p2.y(), p2.z());
        }

        glEnd();
    }
}