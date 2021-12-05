#include "chunk.hpp"

#include <random>

#include "../render/model.hpp"

#include <QGLContext>

CChunk::CChunk(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
    m_x = x;
    m_y = y;
    m_z = z;

    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_sizeZ = sizeZ;

    m_voxels = new uint32_t[m_sizeX * m_sizeY * m_sizeZ];
    memset(m_voxels, 0, m_sizeX * m_sizeY * m_sizeZ * sizeof(uint32_t));
    for ( int y = 7; y >= 0; y-- )
    {
        for ( int x = 0; x < m_sizeX; x++ )
        {
            for ( int z = 0; z < m_sizeZ; z++ )
            {
                m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] = 1;
            }
        }
    }

    m_model = new Model();
    rebuildModel();
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
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return 0;
    }
    return m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}
uint16_t CChunk::getMeta(int x, int y, int z)
{
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return 0;
    }
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
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return;
    }
    m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] = id | (meta << 16);
}

void CChunk::set(int x, int y, int z, uint32_t voxel)
{
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return;
    }
    m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY] = voxel;
}

uint32_t CChunk::get(int x, int y, int z)
{
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return 0;
    }
    return m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}

void CChunk::get(int x, int y, int z, uint32_t &voxel)
{
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        return;
    }
    voxel = m_voxels[x + y * m_sizeX + z * m_sizeX * m_sizeY];
}

void CChunk::get(int x, int y, int z, uint16_t &id, uint16_t &meta)
{
    if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY || z < 0 || z >= m_sizeZ)
    {
        id = 0;
        meta = 0;
        return;
    }
    id = getID(x, y, z);
    meta = getMeta(x, y, z);
}

const int cubeVertices[8][3] = {
	// NORTH +Z
	{ 1, 1, 1 },
	{ 0, 1, 1 },
	{ 0, 0, 1 },
	{ 1, 0, 1 },
	// SOUTH -Z
	{ 0, 1, 0 },
	{ 1, 1, 0 },
	{ 1, 0, 0 },
	{ 0, 0, 0 },
};

const int cubeTriangles[6][4]  = {
	{ 0, 1, 2, 3 }, // N
	{ 5, 0, 3, 6 }, // E
	{ 4, 5, 6, 7 }, // S
	{ 1, 4, 7, 2 }, // W

	{ 5, 4, 1, 0 }, // UP
	{ 3, 2, 7, 6 }	// DN
};

const int dir[6][3] = {
    { 0, 0, 1 },
    { 1, 0, 0 },
    { 0, 0, -1 },
    { -1, 0, 0 },
    { 0, 1, 0 },
    { 0, -1, 0 }
};

void CChunk::rebuildModel()
{
    m_model->m_vertices.clear();
    m_model->m_faces.clear();

    for (int z = 0; z < m_sizeZ; z++)
    {
        for (int y = 0; y < m_sizeY; y++)
        {
            for (int x = 0; x < m_sizeX; x++)
            {
                if (getID(x, y, z) == 0)
                {
                    continue;
                }

                for (int face = 0; face < 6; face++)
                {
                    // test if face is visible, by checking if there's a block in the direction of the face
                    int dx = x + dir[face][0];
                    int dy = y + dir[face][1];
                    int dz = z + dir[face][2];

                    if (getID(dx, dy, dz) != 0)
                    {
                        continue;
                    }

                    // construct the face
                    for (int i = 0; i < 4; i++)
                    {
                        Model::Vertex v;
                        v.x = x + cubeVertices[cubeTriangles[face][i]][0];
                        v.y = y + cubeVertices[cubeTriangles[face][i]][1];
                        v.z = z + cubeVertices[cubeTriangles[face][i]][2];

                        switch (i)
                        {
                            case 0:
                                v.u = 0.0f;
                                v.v = 0.0f;
                                break;
                            case 1:
                                v.u = 1.0f;
                                v.v = 0.0f;
                                break;
                            case 2:
                                v.u = 1.0f;
                                v.v = 1.0f;
                                break;
                            case 3:
                                v.u = 0.0f;
                                v.v = 1.0f;
                                break;
                        }

                        v.nx = dir[face][0];
                        v.ny = dir[face][1];
                        v.nz = dir[face][2];

                        m_model->m_vertices.push_back(v);
                    }

                    m_model->m_faces.push_back(Model::Face{ m_model->m_vertices.size() - 4, m_model->m_vertices.size() - 3, m_model->m_vertices.size() - 2 });
                    m_model->m_faces.push_back(Model::Face{ m_model->m_vertices.size() - 4, m_model->m_vertices.size() - 2, m_model->m_vertices.size() - 1 });
                }
            }
        }
    }

    m_model->update();
}

void CChunk::render(QGLContext *context)
{
    m_model->render(context);
}