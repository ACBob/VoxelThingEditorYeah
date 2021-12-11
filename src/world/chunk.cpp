#include "chunk.hpp"

#include "world.hpp"

#include <random>

#include "editor/defs.hpp"
#include "editor/editorstate.hpp"

#include "render/model.hpp"
#include "render/texture.hpp"

#include <QGLContext>

#include <math.h>

CChunk::CChunk( int x, int y, int z, int sizeX, int sizeY, int sizeZ )
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;

	m_size.x = sizeX;
	m_size.y = sizeY;
	m_size.z = sizeZ;

	m_voxels = new uint32_t[m_size.x * m_size.y * m_size.z];
	memset( m_voxels, 0, m_size.x * m_size.y * m_size.z * sizeof( uint32_t ) );
	for ( int y = 7; y >= 0; y-- )
	{
		for ( int x = 0; x < m_size.x; x++ )
		{
			for ( int z = 0; z < m_size.z; z++ )
			{
				m_voxels[x + y * m_size.x + z * m_size.x * m_size.y] = 1;
			}
		}
	}

	m_lighting = new uint16_t[m_size.x * m_size.y * m_size.z];
	memset( m_lighting, 0, m_size.x * m_size.y * m_size.z * sizeof( uint16_t ) );

	m_world = nullptr;

	m_model = new Model();
}

CChunk::~CChunk() { delete[] m_voxels; }

int CChunk::getX() { return m_pos.x; }
void CChunk::setX( int x ) { m_pos.x = x; }
int CChunk::getY() { return m_pos.y; }
void CChunk::setY( int y ) { m_pos.y = y; }
int CChunk::getZ() { return m_pos.z; }
void CChunk::setZ( int z ) { m_pos.z = z; }

int CChunk::getSizeX() { return m_size.x; }
void CChunk::setSizeX( int sizeX ) { m_size.x = sizeX; }
int CChunk::getSizeY() { return m_size.y; }
void CChunk::setSizeY( int sizeY ) { m_size.y = sizeY; }
int CChunk::getSizeZ() { return m_size.z; }
void CChunk::setSizeZ( int sizeZ ) { m_size.z = sizeZ; }

uint16_t CChunk::getID( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return 0;
	}
	return m_voxels[x + y * m_size.x + z * m_size.x * m_size.y];
}
uint16_t CChunk::getMeta( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return 0;
	}
	return m_voxels[x + y * m_size.x + z * m_size.x * m_size.y] >> 16;
}

void CChunk::setID( int x, int y, int z, uint16_t id ) { set( x, y, z, id, getMeta( x, y, z ) ); }

void CChunk::setMeta( int x, int y, int z, uint16_t meta ) { set( x, y, z, getID( x, y, z ), meta ); }

void CChunk::set( int x, int y, int z, uint16_t id, uint16_t meta )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return;
	}
	m_voxels[x + y * m_size.x + z * m_size.x * m_size.y] = id | ( meta << 16 );
}

void CChunk::set( int x, int y, int z, uint32_t voxel )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return;
	}
	m_voxels[x + y * m_size.x + z * m_size.x * m_size.y] = voxel;
}

uint32_t CChunk::get( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return 0;
	}
	return m_voxels[x + y * m_size.x + z * m_size.x * m_size.y];
}

void CChunk::get( int x, int y, int z, uint32_t &voxel )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		return;
	}
	voxel = m_voxels[x + y * m_size.x + z * m_size.x * m_size.y];
}

void CChunk::get( int x, int y, int z, uint16_t &id, uint16_t &meta )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
	{
		id	 = 0;
		meta = 0;
		return;
	}
	id	 = getID( x, y, z );
	meta = getMeta( x, y, z );
}

void CChunk::set( int i, uint16_t id, uint16_t meta ) { m_voxels[i] = id | ( meta << 16 ); }

uint32_t CChunk::get( int i ) { return m_voxels[i]; }
void CChunk::set( int i, uint32_t voxel ) { m_voxels[i] = voxel; }

uint16_t CChunk::getID( int i ) { return m_voxels[i] & 0xFFFF; }
uint16_t CChunk::getMeta( int i ) { return m_voxels[i] >> 16; }

void CChunk::getID( int i, uint16_t &id ) { id = m_voxels[i] & 0xFFFF; }
void CChunk::getMeta( int i, uint16_t &meta ) { meta = m_voxels[i] >> 16; }

void CChunk::get( int i, uint16_t &id, uint16_t &meta )
{
	id	 = getID( i );
	meta = getMeta( i );
}

void CChunk::get( int i, uint32_t &voxel ) { voxel = m_voxels[i]; }

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

const int cubeTriangles[6][4] = {
	{ 0, 1, 2, 3 }, // N
	{ 5, 0, 3, 6 }, // E
	{ 4, 5, 6, 7 }, // S
	{ 1, 4, 7, 2 }, // W

	{ 5, 4, 1, 0 }, // UP
	{ 3, 2, 7, 6 }	// DN
};

const int dir[6][3] = { { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, -1 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 } };

void CChunk::rebuildModel()
{
	m_model->m_vertices.clear();
	m_model->m_faces.clear();

	calculateLighting();

	for ( int z = 0; z < m_size.z; z++ )
	{
		for ( int y = 0; y < m_size.y; y++ )
		{
			for ( int x = 0; x < m_size.x; x++ )
			{
				if ( getID( x, y, z ) == 0 )
				{
					continue;
				}

				for ( int face = 0; face < 6; face++ )
				{
					// test if face is visible, by checking if there's a block in the direction of the face
					int dx = x + dir[face][0];
					int dy = y + dir[face][1];
					int dz = z + dir[face][2];

					if ( getID( dx, dy, dz ) != 0 )
					{
						continue;
					}

					// get lighting
					uint16_t light = getLighting( dx, dy, dz );
					uint8_t red = ( light >> 12 ) & 0xFF;
					uint8_t green = ( light >> 8 ) & 0xFF;
					uint8_t blue = ( light >> 4 ) & 0xFF;
					uint8_t sky = ( light >> 0 ) & 0xFF;

					// construct the face
					for ( int i = 0; i < 4; i++ )
					{
						Model::Vertex v;
						v.x = m_pos.x * m_size.x + x + cubeVertices[cubeTriangles[face][i]][0];
						v.y = m_pos.y * m_size.y + y + cubeVertices[cubeTriangles[face][i]][1];
						v.z = m_pos.z * m_size.z + z + cubeVertices[cubeTriangles[face][i]][2];
						v.r = qBound(0.1f, fmaxf( red / 16.0f, sky / 16.0f ) * 1.5f, 1.0f);
						v.g = qBound(0.1f, fmaxf( green / 16.0f, sky / 16.0f ) * 1.5f, 1.0f);
						v.b = qBound(0.1f, fmaxf( blue / 16.0f, sky / 16.0f ) * 1.5f, 1.0f);

						Vector4f uv;
						if ( m_editorState != nullptr )
						{
							uv = render::getUV( m_editorState->blockDefs, getID( x, y, z ) );
							uv.y -= 1 / 16.0f; // HACK: otherwise all textures are offset by 1/16
							uv.w -= 1 / 16.0f;
						}
						else
							uv = { 0, 0, 1, 1 };

						switch ( i )
						{
							case 0:
								v.u = uv.x;
								v.v = uv.y;
								break;
							case 1:
								v.u = uv.z;
								v.v = uv.y;
								break;
							case 2:
								v.u = uv.z;
								v.v = uv.w;
								break;
							case 3:
								v.u = uv.x;
								v.v = uv.w;
								break;
						}

						v.nx = dir[face][0];
						v.ny = dir[face][1];
						v.nz = dir[face][2];

						m_model->m_vertices.push_back( v );
					}

					m_model->m_faces.push_back( Model::Face{ m_model->m_vertices.size() - 4,
															 m_model->m_vertices.size() - 3,
															 m_model->m_vertices.size() - 2 } );
					m_model->m_faces.push_back( Model::Face{ m_model->m_vertices.size() - 4,
															 m_model->m_vertices.size() - 2,
															 m_model->m_vertices.size() - 1 } );
				}
			}
		}
	}

	m_model->update();
}

void CChunk::render( QGLContext *context ) { m_model->render( context ); }

#define setIfAir( x, y, z, id, meta )                                                                                  \
	if ( getID( x, y, z ) == 0 )                                                                                       \
	{                                                                                                                  \
		setID( x, y, z, id );                                                                                          \
		setMeta( x, y, z, meta );                                                                                      \
	}

void CChunk::simulateLiquid()
{

	QList<Vector3i> liquidPositions;

	for ( int z = 0; z < m_size.z; z++ )
	{
		for ( int y = 0; y < m_size.y; y++ )
		{
			for ( int x = 0; x < m_size.x; x++ )
			{
				uint16_t id = getID( x, y, z );
				if ( id == 0 )
				{
					continue;
				}

				// if it is marked as liquid by the block definition, add it to the list
				if ( m_editorState->blockDefs->value( id ).isLiquid )
				{
					if ( m_editorState->blockDefs->value( id ).liquidSource == 0 )
						continue;
					liquidPositions.append( Vector3i( x, y, z ) );
				}
			}
		}
	}

	// simulate the liquid
	for ( int i = 0; i < liquidPositions.size(); i++ )
	{
		Vector3i pos = liquidPositions[i];
		uint16_t id	 = getID( pos.x, pos.y, pos.z );

		// get the liquid level from the metadata
		uint16_t level = getMeta( pos.x, pos.y, pos.z );

		// simulate the liquid

		// test if the block is on the floor
		bool onFloor = ( getID( pos.x, pos.y - 1, pos.z ) != 0 && getID( pos.x, pos.y - 1, pos.z ) != id );

		// if the level is 0, continue
		if ( level == 0 && onFloor )
		{
			continue;
		}

		uint16_t flowBlock = m_editorState->blockDefs->value( id ).liquidFlow;

		if ( onFloor && level > 0 )
		{
			// Flow in all directions
			setIfAir( pos.x + 1, pos.y, pos.z, flowBlock, level - 1 );
			setIfAir( pos.x - 1, pos.y, pos.z, flowBlock, level - 1 );
			setIfAir( pos.x, pos.y, pos.z + 1, flowBlock, level - 1 );
			setIfAir( pos.x, pos.y, pos.z - 1, flowBlock, level - 1 );

			continue;
		}

		// if the block is not on the floor, flow down
		setIfAir( pos.x, pos.y - 1, pos.z, flowBlock,
				  (uint16_t)qBound( 0, level + 1, (int)m_editorState->blockDefs->value( id ).metaMax ) );
	}
}

// first 4 bits are used for skylight
// last 12 bits are RGB
// for 16 bit lighting

uint8_t CChunk::getSkyLight( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
		return 15;

	return m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] >> 4;
}

uint16_t CChunk::getBlockLight( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
		return 15;

	return m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] & 0x0F;
}

uint16_t CChunk::getLighting( int x, int y, int z )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
		return 0xFFFF;

	return m_lighting[x + y * m_size.x + z * m_size.x * m_size.y];
}

void CChunk::setSkyLight( int x, int y, int z, uint8_t light )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
		return;

	m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] = ( m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] & 0xF0 ) | light;
}

void CChunk::setBlockLight( int x, int y, int z, uint16_t light )
{
	if ( x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z )
		return;

	m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] = ( m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] & 0x0F ) | light;
}

void CChunk::setBlockLight(int x, int y, int z, uint8_t red, uint8_t green, uint8_t blue)
{
	if (x < 0 || x >= m_size.x || y < 0 || y >= m_size.y || z < 0 || z >= m_size.z)
		return;

	uint16_t light = (red << 12) | (green << 8) | (blue << 4) | (m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] & 0x0F);
	m_lighting[x + y * m_size.x + z * m_size.x * m_size.y] = light;
}



void CChunk::calculateLighting() {
	// Reading material
	// https://0fps.net/2018/02/21/voxel-lighting/
	// https://web.archive.org/web/20210429192404/https://www.seedofandromeda.com/blogs/29-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-1
	// https://web.archive.org/web/20210429192404/https://www.seedofandromeda.com/blogs/30-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-2
	// https://web.archive.org/web/20181212111600/https://notch.tumblr.com/post/434902871/per-request-this-is-how-the-new-lighting-will

	qDebug() << "Calculating lighting";

	// Sky light is the last 4 bits of the lighting buffer
	// Block light is the first 12 bits, each 4 bits is a channel
	// So RGB is 12 bits each, and the last 4 bits are sky light
	// Essentially RGBA, where A is sky light

	memset( m_lighting, 0, m_size.x * m_size.y * m_size.z * sizeof( uint16_t ) );

	// Step 1. Sunlight propagation
	// test the chunk above us
	CChunk *chunkAbove = m_editorState->world->getChunk( m_pos + Vector3i( 0, 1, 0 ) );
	uint8_t *skyLight = new uint8_t[m_size.x * m_size.z];
	if (chunkAbove != nullptr) {
		for (int z = 0; z < m_size.z; z++) {
			for (int x = 0; x < m_size.x; x++) {
				skyLight[z * m_size.x + x] = chunkAbove->getSkyLight(x, 0, z);
			}
		}
	}
	else {
		// if we're above 0, we can guess there'll be skylight
		if (m_pos.y + 1 > 0) {
			for (int z = 0; z < m_size.z; z++) {
				for (int x = 0; x < m_size.x; x++) {
					skyLight[z * m_size.x + x] = 15;
				}
			}
		}
		else {
			for (int z = 0; z < m_size.z; z++) {
				for (int x = 0; x < m_size.x; x++) {
					skyLight[z * m_size.x + x] = 0;
				}
			}
		}
	}

	// Now propogate that sunlight into us
	for (int z = 0; z < m_size.z; z++) {
		for (int x = 0; x < m_size.x; x++) {
			uint8_t sky = skyLight[z * m_size.x + x];
			if (sky > 0) {
				// loop down to the bottom of the chunk, stopping if we hit a block
				for (int y = m_size.y - 1; y >= 0; y--) {
					uint16_t id = getID(x, y, z);
					if (id != 0) {
						break;
					}
					else {
						setSkyLight(x, y, z, sky);
					}
				}
			}
		}
	}
}