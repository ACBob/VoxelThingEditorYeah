#include "model.hpp"

// TODO: modern OpenGL

#include <QDebug>

#include <QOpenGLFunctions>

Model::Model(QObject *parent) : QObject(parent)
{
}

Model::~Model()
{
}

void Model::render(QGLContext *context)
{
    context->makeCurrent();

    glVertexPointer( 3,
                    GL_FLOAT,
                    sizeof(Model::Vertex),
                    m_vertices.data());
    glTexCoordPointer( 2,
                        GL_FLOAT,
                        sizeof(Model::Vertex),
                        &m_vertices.data()[0].u );
    
    glNormalPointer( GL_FLOAT,
                    sizeof(Model::Vertex),
                    &m_vertices.data()[0].nx );

    glColor3f(1.0f, 1.0f, 1.0f);
    
    glDrawElements(GL_TRIANGLES, m_faces.size() * 3, GL_UNSIGNED_INT, m_faces.data());

    // glDrawArrays(GL_POINTS, 0, m_vertices.size());
}
void Model::loadFromObj(const QString &fileName)
{
    qDebug() << "TODO: loadFromObj";
}

bool Model::saveToObj(const QString &fileName)
{
    qDebug() << "TODO: saveToObj";
    return false;
}

void Model::update()
{

}