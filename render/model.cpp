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

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < m_faces.size(); i++) {
        Face &face = m_faces[i];
        Vertex &v1 = m_vertices[face.v1];
        Vertex &v2 = m_vertices[face.v2];
        Vertex &v3 = m_vertices[face.v3];

        glNormal3f(v1.nx, v1.ny, v1.nz);
        glTexCoord2f(v1.u, v1.v);
        glVertex3f(v1.x, v1.y, v1.z);

        glNormal3f(v2.nx, v2.ny, v2.nz);
        glTexCoord2f(v2.u, v2.v);
        glVertex3f(v2.x, v2.y, v2.z);

        glNormal3f(v3.nx, v3.ny, v3.nz);
        glTexCoord2f(v3.u, v3.v);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
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