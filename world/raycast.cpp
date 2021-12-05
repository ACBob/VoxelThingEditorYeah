#include "raycast.hpp"

#include "chunk.hpp"

#include <math.h>

CRaycast::CRaycast(QObject *parent) :
    QObject(parent)
{
    m_start = QVector3D(0, 0, 0);
    m_fLength = 5.0f;
    m_direction = QVector3D(0, 0, 1);
}

CRaycast::~CRaycast()
{

}

std::pair<QVector3D, QVector3D> CRaycast::cast(CChunk *chunk)
{
    QVector3D ray = m_start;
    QVector3D oRay; // Used for figuring out the normal

    const float step = 0.01f;
    float i = 0.0f;

    while (i < m_fLength)
    {
        oRay = ray;
        ray = m_start + m_direction * i;
        i += step;

        int x = floorf(ray.x());
        int y = floorf(ray.y());
        int z = floorf(ray.z());

        if (chunk->getID(x, y, z) > 0)
        {
            QVector3D normal = ray - QVector3D(x, y, z);
            normal.normalize();
            return {ray, normal};
        }
    }

    return { QVector3D(0, 0, 0), QVector3D(0, 0, 0) };
}

std::pair<QVector3D, QVector3D> CRaycast::cast(CChunk *chunk, QVector3D start, QVector3D direction, float length)
{
    m_start = start;
    m_direction = direction;
    m_fLength = length;

    return cast(chunk);
}