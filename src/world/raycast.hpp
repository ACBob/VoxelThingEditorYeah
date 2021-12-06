// -- Raycast
// Casts a ray from a point in space to a point in space.
// Stopped by either a given chunk, or a chunk in the world.

#include <QObject>

#include <QVector3D>

class CChunk;
// class CWorld;

class CRaycast : public QObject
{
    Q_OBJECT

    public:
        CRaycast(QObject *parent = 0);
        ~CRaycast();

        QVector3D m_start;
        float m_fLength;
        QVector3D m_direction;

        std::pair<QVector3D, QVector3D> cast(CChunk *chunk);
        std::pair<QVector3D, QVector3D> cast(CChunk *chunk, QVector3D start, QVector3D direction, float length);
        // QVector3D cast(CWorld *world);
        // QVector3D cast(CWorld *world, QVector3D start, QVector3D direction, float length);
};