#include "world.hpp"

#include <QMap>
#include <QVector3D>

#include "chunk.hpp"

CWorld::CWorld(QObject *parent) : QObject(parent)
{
    m_chunkSize = QVector3D(DEF_CHUNK_SIZE_X, DEF_CHUNK_SIZE_Y, DEF_CHUNK_SIZE_Z);
}

CWorld::~CWorld() {
    for (QVector3D p : m_chunks.keys())
        delete m_chunks.value(p);
}

CChunk *CWorld::getChunk(int x, int y, int z) {
    return getChunk(QVector3D(x,y,z));
}

CChunk *CWorld::getChunk(const QVector3D &pos) {
    return m_chunks.value(pos, nullptr); // QMaps rule
}

CChunk *CWorld::createChunk(int x, int y, int z) {
    return createChunk(QVector3D(x, y, z));
}

CChunk *CWorld::createChunk(const QVector3D &pos) {
    if (getChunk(pos) != nullptr)
        return nullptr; // Couldn't create it

    CChunk *c = new CChunk(pos.x(), pos.y(), pos.z(), m_chunkSize.x(), m_chunkSize.y(), m_chunkSize.z());
    
    m_chunks[pos] = c;

    return c;
}

CChunk *CWorld::getOrCreateChunk(int x, int y, int z) {
    return getOrCreateChunk(QVector3D(x, y, z));
}

CChunk *CWorld::getOrCreateChunk(const QVector3D &pos) {
    CChunk *c = getChunk(pos);
    return c != nullptr ? c : createChunk(pos);
}

void CWorld::render(QGLContext *ctx) {
    for (CChunk *c : m_chunks.values())
        c->render(ctx);
}