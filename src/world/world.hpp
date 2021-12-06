// -- World.hpp
// Holds chunks in a QMap<Vector3f, CChunk*>
// and also some other useful things (like rendering a world)
// At some point, will also hold entities!

#include <QObject>
#include <QMap>
#include "vector.hpp"

#pragma once

class CChunk;
class QGLContext;

class CWorld : public QObject {
    
    Q_OBJECT

    public:
        CWorld(QObject *parent = 0);
        ~CWorld();

        CChunk *getChunk(int x, int y, int z);
        CChunk *getChunk(const Vector3i& coord);

        CChunk *createChunk(int x, int y, int z);
        CChunk *createChunk(const Vector3i &coord);

        CChunk *getOrCreateChunk(int x, int y, int z);
        CChunk *getOrCreateChunk(const Vector3i &coord);

        CChunk *getChunkWorldPos(int x, int y, int z);
        CChunk *getChunkWorldPos(const Vector3i &coord);

        // Converts from Chunk grid coords to world pos
        Vector3f chunkPosToWorldPos(const Vector3f& pos);

        // Converts from world pos to Chunk grid coords
        Vector3f worldPosToChunkPos(const Vector3f& pos);

        // Converts from Chunk grid coords to world pos
        void chunkPosToWorldPos(int &x, int &y, int &z);

        // Converts from world pos to Chunk grid coords
        void worldPosToChunkPos(int &x, int &y, int &z);

        void render(QGLContext *ctx);

        uint16_t getID(int x, int y, int z);
        void setID(int x, int y, int z, uint16_t id);

        uint16_t getMeta(int x, int y, int z);
        void setMeta(int x, int y, int z, uint16_t meta);

        void get(int x, int y, int z, uint16_t &id, uint16_t &meta);
        void set(int x, int y, int z, uint16_t id, uint16_t meta);

    private:
        QMap<Vector3i, CChunk*> m_chunks;

        // Luckily, chunk sizes are per world, not per chunk.
        // The first map format had it be per chunk, and that's silly.
        // Even luckier is that I rectified it *before* working on this and was able to avoid the headache that is.
        Vector3i m_chunkSize;
};