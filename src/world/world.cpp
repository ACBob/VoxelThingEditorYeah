#include "world.hpp"

#include <QMap>
#include "vector.hpp"

#include "chunk.hpp"

#include <QDebug>

CWorld::CWorld(QObject *parent) : QObject(parent)
{
    m_chunkSize = Vector3f(DEF_CHUNK_SIZE_X, DEF_CHUNK_SIZE_Y, DEF_CHUNK_SIZE_Z);
}

CWorld::~CWorld() {
    for (Vector3f p : m_chunks.keys())
        delete m_chunks.value(p);
}

CChunk *CWorld::getChunk(int x, int y, int z) {
    return getChunk(Vector3f(x,y,z));
}

CChunk *CWorld::getChunk(const Vector3i &pos) {
    return m_chunks.value(pos, nullptr); // QMaps rule
}

CChunk *CWorld::createChunk(int x, int y, int z) {
    return createChunk(Vector3f(x, y, z));
}

CChunk *CWorld::createChunk(const Vector3i &pos) {
    if (m_chunks.contains(pos))
        return nullptr; // Couldn't create it

    CChunk *c = new CChunk(pos.x, pos.y, pos.z, m_chunkSize.x, m_chunkSize.y, m_chunkSize.z);
    
    m_chunks.insert(pos, c);

    qDebug() << "Created chunk at " << pos;
    qDebug() << "Chunk count: " << m_chunks.size();
    qDebug() << "Chunk ptr value: " << c;

    return c;
}

CChunk *CWorld::getOrCreateChunk(int x, int y, int z) {
    return getOrCreateChunk(Vector3f(x, y, z));
}

CChunk *CWorld::getOrCreateChunk(const Vector3i &pos) {
    CChunk *c = getChunk(pos);
    return c != nullptr ? c : createChunk(pos);
}

CChunk *CWorld::getChunkWorldPos( int x, int y, int z ) {
    return getChunkWorldPos(Vector3f(x, y, z));
}

CChunk *CWorld::getChunkWorldPos(const Vector3i &pos) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    worldPosToChunkPos(x, y, z);

    return getChunk(x, y, z);
}

Vector3f CWorld::chunkPosToWorldPos(const Vector3f &pos) {
    return pos * m_chunkSize;
}

Vector3f CWorld::worldPosToChunkPos(const Vector3f &pos) {
    return pos / m_chunkSize;
}

void CWorld::chunkPosToWorldPos(int &x, int &y, int &z) {
    Vector3f pos = chunkPosToWorldPos(Vector3f(x, y, z));
    x = pos.x;
    y = pos.y;
    z = pos.z;
}

void CWorld::worldPosToChunkPos(int &x, int &y, int &z) {
    Vector3f pos = worldPosToChunkPos(Vector3f(x, y, z));
    x = pos.x;
    y = pos.y;
    z = pos.z;
}


void CWorld::render(QGLContext *ctx) {
    // qDebug() << "CWorld::render()";
    // qDebug() << "Chunk count: " << m_chunks.size();
    for (CChunk *c : m_chunks.values())
        c->render(ctx);
}

uint16_t CWorld::getID( int x, int y, int z ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr)
        return 0;

    return c->getID(x, y, z);
}

void CWorld::setID( int x, int y, int z, uint16_t id ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr)
        return;

    c->setID(x, y, z, id);
}

uint16_t CWorld::getMeta( int x, int y, int z ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr)
        return 0;

    return c->getMeta(x, y, z);
}

void CWorld::setMeta( int x, int y, int z, uint16_t meta ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr)
        return;

    c->setMeta(x, y, z, meta);
}

void CWorld::get( int x, int y, int z, uint16_t &id, uint16_t &meta ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr) {
        id = 0;
        meta = 0;
        return;
    }

    c->get(x, y, z, id, meta);
}

void CWorld::set( int x, int y, int z, uint16_t id, uint16_t meta ) {
    // get in chunk coordinates
    int cx = x;
    int cy = y;
    int cz = z;
    worldPosToChunkPos(cx, cy, cz);

    x -= cx * m_chunkSize.x;
    y -= cy * m_chunkSize.y;
    z -= cz * m_chunkSize.z;

    CChunk *c = getChunk(cx, cy, cz);
    if (c == nullptr)
        return;

    c->set(x, y, z, id, meta);
}