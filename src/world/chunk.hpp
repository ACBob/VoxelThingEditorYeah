// -- Chunk

// Concepts:
// "Block" - uint16_t Block ID.
// "Meta" - uint16_t Block Meta.
// "Voxel" - uint32_t Block ID + Meta.
// "Chunk" - Collection of Blocks.

#include <QObject>

// Default chunk size
// Used if the map doesn't specify, or on new maps
#define DEF_CHUNK_SIZE_X 16
#define DEF_CHUNK_SIZE_Y 16
#define DEF_CHUNK_SIZE_Z 16

class Model; // Forward declaration
class QGLContext; // Forward declaration

#include "../editor/blockdefs.hpp"

class CChunk : public QObject
{
    Q_OBJECT;
    public:
        CChunk(int x, int y, int z, int sizeX = DEF_CHUNK_SIZE_X, int sizeY = DEF_CHUNK_SIZE_Y, int sizeZ = DEF_CHUNK_SIZE_Z);
        ~CChunk();

        int getX();
        void setX(int x);
        int getY();
        void setY(int y);
        int getZ();
        void setZ(int z);

        int getSizeX();
        void setSizeX(int sizeX);
        int getSizeY();
        void setSizeY(int sizeY);
        int getSizeZ();
        void setSizeZ(int sizeZ);


        uint16_t getID(int x, int y, int z);
        uint16_t getMeta(int x, int y, int z);

        void setID(int x, int y, int z, uint16_t id);
        void setMeta(int x, int y, int z, uint16_t meta);

        void set(int x, int y, int z, uint16_t id, uint16_t meta);
        void set(int x, int y, int z, uint32_t voxel);

        uint32_t get(int x, int y, int z);
        void get(int x, int y, int z, uint16_t& id, uint16_t& meta);
        void get(int x, int y, int z, uint32_t& voxel);

        void rebuildModel();

        void render(QGLContext *context);

        BlockDefs *m_blockDefs;

    private:
        uint32_t *m_voxels;

        int m_x;
        int m_y;
        int m_z;

        int m_sizeX;
        int m_sizeY;
        int m_sizeZ;

        Model *m_model;
};
