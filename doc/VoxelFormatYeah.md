# VoxelFormatYeah - Generic world format for VoxelThingYeah
## Specification

## Directory Structure
A world has the following directory structure:
```
world/
├ meta.toml
├─ <dimension_name>/
│   ├ entities.toml
│   └ chunks/
│       ├ chunk_x_y_z.dat
│       ├ etc...
│       └ chunk_x_y_z.dat
├ players.toml
└ editor.toml
```

## meta.toml
`meta.toml` holds metadata about the world. It also defines the dimensions.
```toml
[meta]
name = "My World" # String
version = 0x1 # UInt32
seed = 69420 # Int32
game = "Meegreef" # Holds the name of the game that created the world, may be used by anyone loading to find block definitions and entity definitions. Required. Although if absent, games may attempt to load the world anyway, blindly assuming that the world is intended for the game. They should create a backup of the world in such a case, before saving.
chunk_sizes = [16, 16, 16] # Array of three UInt8s, the size of each chunk in blocks. Required. Cannot be smaller than 1, cannot be larger than 255.
creation_time = "2021-11-31" # Either a Unix timestamp or an ISO 8601 date string. Required.

# The entire author field is optional.
[author]
name = "Baob Koiss" # String
email = "foobar@example.com" # String
url = "https://example.com" # String
license = "CC-BY-SA-4.0" # String (Defaults to "CC-BY-SA-4.0" if not specified)
description = "This is a description of the world." # String

[dimensions.<dimension_name>]
name = "Overworld" # String, the name of the dimension. Required.
additional_seed = 0 # Int32, an additional seed to use when generating chunks. Optional.
time = 0 # Int32, the time in ticks. 0-24000. Optional, though assumed to be 0 if not specified.
day = 0 # Int16, the day in the game year. 0-365. Optional, though assumed to be 0 if not specified.
year = 0 # Int16, the year in the game. 0-65535. Optional, though assumed to be 0 if not specified.
weather = "clear" # String, the weather. Optional, though assumed to be "clear" if not specified. Even if a game does not have weather, "clear" should be understood as "no weather", or ignored.
weather_length = 0 # Int32, how long is left in the current weather. Optional, though assumed to be 0 if not specified.
force_load = true # Boolean, if true the game will always keep 0,0,0 loaded. Optional, though assumed to be false if not specified. Games may also send 0,0,0 to joining clients regardless of distance if true.
max_chunk_pos = [] # Array of three Int16s, the maximum chunk position in the dimension, in both directions (goes from -n to +n). Chunks beyond these positions cannot exist, and players cannot go beyond these positions. Optional, though assumed to be 2 ^ 15 - 1 if not specified. Cannot be larger than 2 ^ 15 - 1, or smaller than 1.
```

## players.toml
`players.toml` holds information about the players in the world.
```toml
[<username>] # The key is the username of the player.
team = "" # String, the team the player is on. Optional.
entity_uuid = "" # Int64, the UUID of the entity that the player is controlling. If not specified, it is assumed that an entity will be created for the player.
spawn_position = [0, 0, 0] # Array of three Int32s, the position of the player's spawn point. Optional. For games where this does not make sense, it may be ignored.
password = "" # Hashed password to lock the username. Optional, but generally players will want one to keep their username safe.
```

## editor.toml
`editor.toml` holds some information last used by the editor.
```toml
[editor]
camera_position = [0, 0, 0] # Array of three Float32s, the position of the camera. Optional.
camera_rotation = [0, 0] # Pitch, yaw. Array of two Float32s, the rotation of the camera. Optional.
camera_zoom = 1.0 # Float32, the zoom of the camera. Optional.
```

## Dimensions
A dimension is a collection of chunks and entities.

A world can have an arbitrary number of dimensions, but individual games may have their own limit. Meegreef's limit is `65536`, which should be more than enough for anyone. Generally, a safe bet is `255`. Games will simply not load dimensions that are out of bounds, and will snap any entities that are set to be out of bounds into the default dimension. This does mean dimensions outside of acceptable bounds will still be scanned for entities.toml, but they will not be loaded.

Dimensions are defined in `meta.toml`. Their chunk data is then loaded from a folder inside the world folder. The folder name is the name of the dimension.

### entities.toml
`entities.toml` holds information about the entities in the dimension.
```toml
[<entity_uuid>] # The key is the UUID of the entity.
type = "player" # String, the type of the entity. Required.
name = "" # String, the name of the entity. For use with the I/O system. Optional, assumed to be "" if not specified.
position = [0, 0, 0] # Array of three Float32s, the position of the entity. Optional, assumed to be [0, 0, 0] if not specified.
rotation = [0, 0] # Pitch, yaw (0-359). Array of two Float32s, the rotation of the entity. Optional, assumed to be [0, 0] if not specified.
velocity = [0, 0, 0] # Array of three Float32s, the velocity of the entity. Optional, assumed to be [0, 0, 0] if not specified.

[<entity_uuid>.metadata] # Holds the entity type's metadata, and/or additional data. i.e in the case of the player, it might be inventory and health, plus some modded information. Optional, assumed to be absent (and therefore entity defaults will be used) if not specified.
# examples
health = 100
inventory = [ "..." ]
```

### chunks/chunk_x_y_z.dat
`chunks/chunk_x_y_z.dat` holds the data for a chunk. The 'x', 'y', and 'z' are the chunk coordinates. Every chunk gets one of these files.

Contrary to the rest of the files, the chunk is a binary blob :trollface:, that may be compressed with zlib. Compression may help with large worlds that have silly chunk sizes.

```
The first 4 bytes are the header. It is a magic string, simply reading "VTYG", which stands for VoxelThingYeah Generic.

After that, the chunk data is different depending on what the chunk has.

Empty chunks will be the word "EMPTY". This is the case for chunks that are made of air.

Ungenerated chunks are the word "UNGENERATED", and will most likely have an array of blocks to replace the blocks with, i.e a structure that is not generated yet. This array offsets the IDs by 1, where 0x00 means do not replace the block, and 0x01 means block id 0x00.

Chunks fully comprised of one block may be "FILLED", followed by the block ID. Metadata is included seperately, as one array of bytes chunk_width * chunk_height * chunk_depth.

Chunks that don't match any of the above will be a flat array of bytes. chunk_width * chunk_height * chunk_depth * 4, where each entry is a pair of bytes. the first two bytes are the block ID, and the second two bytes are the metadata. Lighting for the chunk is included after this array, as chunk_width * chunk_height * chunk_depth * 2 bytes. Lighting is RGBA, with 4 bits per channel.
```


## Original Draft
The original draft was written as a comment shortly before I wrote this markdown file. It is included for posterity.
```cpp
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
```