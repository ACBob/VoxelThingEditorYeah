// -- World.hpp
// Holds chunks in a QMap<QVector3D, CChunk*>
// and also some other useful things (like rendering a world)
// At some point, will also hold entities!

#include <QObject>
#include <QMap>
#include <QVector3D>

#pragma once

class CChunk;
class QGLContext;

// Tiny class that lets me store vectors as QMap keys
// This adds a bit of time wasting I assume, but eh
struct MIDVEC {
    float x, y, z;

    MIDVEC(QVector3D p) {
        x = p.x();
        y = p.y();
        z = p.z();
    }

    operator QVector3D() const {
        return QVector3D(x, y, z);
    }

    bool operator<(const MIDVEC &o) const {
        return x < o.x && y < o.y && z < o.z;
    }
};

class CWorld : public QObject {
    
    Q_OBJECT

    public:
        CWorld(QObject *parent = 0);
        ~CWorld();

        CChunk *getChunk(int x, int y, int z);
        CChunk *getChunk(const QVector3D& coord);

        CChunk *createChunk(int x, int y, int z);
        CChunk *createChunk(const QVector3D &coord);

        CChunk *getOrCreateChunk(int x, int y, int z);
        CChunk *getOrCreateChunk(const QVector3D &coord);

        // Converts from Chunk grid coords to world pos
        QVector3D chunkPosToWorldPos(const QVector3D& pos);

        // Converts from world pos to Chunk grid coords
        QVector3D worldPosToChunkPos(const QVector3D& pos);

        void render(QGLContext *ctx);

    private:
        QMap<MIDVEC, CChunk*> m_chunks;

        // Luckily, chunk sizes are per world, not per chunk.
        // The first map format had it be per chunk, and that's silly.
        // Even luckier is that I rectified it *before* working on this and was able to avoid the headache that is.
        QVector3D m_chunkSize;
};