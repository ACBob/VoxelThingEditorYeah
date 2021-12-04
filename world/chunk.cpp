#include "chunk.hpp"

#include <random>

CChunk::CChunk(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
    m_x = x;
    m_y = y;
    m_z = z;

    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_sizeZ = sizeZ;

    m_voxels = new uint32_t[m_sizeX * m_sizeY * m_sizeZ];
    for (int i = 0; i < m_sizeX * m_sizeY * m_sizeZ; i++)
    {
        m_voxels[i] = rand() % 2;
    }
}

CChunk::~CChunk()
{
    delete[] m_voxels;
}

int CChunk::getX() { return m_x; }
void CChunk::setX(int x) { m_x = x; }
int CChunk::getY() { return m_y; }
void CChunk::setY(int y) { m_y = y; }
int CChunk::getZ() { return m_z; }
void CChunk::setZ(int z) { m_z = z; }

int CChunk::getSizeX() { return m_sizeX; }
void CChunk::setSizeX(int sizeX) { m_sizeX = sizeX; }
int CChunk::getSizeY() { return m_sizeY; }
void CChunk::setSizeY(int sizeY) { m_sizeY = sizeY; }
int CChunk::getSizeZ() { return m_sizeZ; }
void CChunk::setSizeZ(int sizeZ) { m_sizeZ = sizeZ; }

uint16_t CChunk::getID(int x, int y, int z)
{
    return m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}
uint16_t CChunk::getMeta(int x, int y, int z)
{
    return m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] >> 16;
}

void CChunk::setID(int x, int y, int z, uint16_t id)
{
    set(x, y, z, id, getMeta(x, y, z));
}

void CChunk::setMeta(int x, int y, int z, uint16_t meta)
{
    set(x, y, z, getID(x, y, z), meta);
}

void CChunk::set(int x, int y, int z, uint16_t id, uint16_t meta)
{
    m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] = id | (meta << 16);
}

void CChunk::set(int x, int y, int z, uint32_t voxel)
{
    m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] = voxel;
}

uint32_t CChunk::get(int x, int y, int z)
{
    return m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}

void CChunk::get(int x, int y, int z, uint32_t &voxel)
{
    voxel = m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}

void CChunk::get(int x, int y, int z, uint16_t &id, uint16_t &meta)
{
    id = getID(x, y, z);
    meta = getMeta(x, y, z);
}