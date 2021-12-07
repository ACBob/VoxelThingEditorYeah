// -- WorldFormat
// Provides a way to implement more world formats nicely

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
// Duely named "VoxelFormatYeah"
/*
	Gone are the days of binary blobs.

	"meta.toml" - holds the world metadata
		[world]
		name = String
		seed = Int32
		version = Int32
		chunk_sizes = [Int32, Int32, Int32] (Cannot be negative or zero)

		[dimensions.<dimension>]
		time = Int32 (0-24000)
		day = Int32 (0-365)
		year = Int32 (0-9999)
		weather = Int32 (0-3)
		weather_length = Int32 (0-24000) (how long the weather has left)

	"players.toml" - holds player data
		[username]
		password = String, A hash of the password
		permissions = ???, The key is reserved for when it has a use.
	
	<dimension>/ - The name of the dimension
		"entities.toml" - Holds the entity data for every entity in the map
			[UUID]
			name = String
			position = [Float, Float, Float]
			rotation = [Float, Float]
			velocity = [Float, Float, Float]
			type = String (One of the entity types listed in the defs file)
		chunks/
			"chunk_x_y_z.dat" - The chunk data, sadly a binary blob
				3 bytes that say "VTY"
				The file data can then be "EMPTY" or a flat array of blocks
				The block ID is 2 bytes, the block meta is 2 bytes
				They come in pairs, the first is the block ID, the second is the block meta
*/