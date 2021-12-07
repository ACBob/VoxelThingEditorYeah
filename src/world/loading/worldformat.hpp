// -- WorldFormat
// Provides a way to implement more world formats nicely
// Each function is given a pointer to the world to load into,
// And the filepath of the world folder.

#include <QString>

class CWorld;
class QFile;

class WorldFormat {
	public:
    // Loads into the given world
    // Is also given the file data
    virtual bool Load(CWorld *world, QString filePath) = 0;
    // The saved file should use the filename!
    virtual bool Save(CWorld *world, QString filename) = 0;
};

// MEEGREEF VERSION 1
/*
	Header - 4 bytes that spell "MEEG"
	Version - 1 byte (Should equal 1)
	World name - up to 32 bytes

	Block Palette size - 1 byte
	Block Palette - Palette size pairs of (id, mapped id)
	  id - 2 bytes
	  mapped id - 2 bytes

	Chunk count - 4 bytes
	Chunk data - Array of chunks, each chunk is written as:
		Chunk position - 4 bytes for X, Y, Z
		Chunk data - Chunk size X*Y*Z :
		 Block ID - 2 bytes
		 Block Meta - 2 bytes
		Chunk data may be vacant if the chunk is empty, instead marked by "EMPTY"
	
	Time of day - 2 bytes
*/

class Meegreef1Format final : public WorldFormat {
	public:
    bool Load(CWorld *world, QString filePath);
    bool Save(CWorld *world, QString filename);
};

// VoxelThingYeah Generic Format
// see docs/VoxelFormatYeah.md
#define VTYG_CURRENT_VERSION 1
#define VTYG_OLDEST_SUPPORTED_VERSION 1

class VoxelFormatYeah final : public WorldFormat {
	public:
	bool Load(CWorld *world, QString filePath);
	bool Save(CWorld *world, QString filename);
};