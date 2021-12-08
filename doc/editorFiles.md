# Editor files

## Contents
 - [games.toml](#games.toml)
 - [game.toml](#game.toml)
 - [palette.toml](#palette.toml)
 - [entitypalette.toml](#entitypalette.toml)


## games.toml
The editor uses the file 'games.toml' to get information of a game. It stores where the game's files are, what the ideal working directory is, and what the game's name is. It is generally reccomended that you simply place a `game.toml` path for each game, if it exists. Using `games.toml` exclusively is for testing purposes.

```toml
[<game_name>]
path = "/home/foo/.bar/" # An absolute path to the game's files
working_dir = "bar" # The ideal working directory, relative to the game's path
texture_path = "textures" # The path to the game's textures, relative to the game's path
file = "foobar.toml" # a path to the game.toml format file
name = "Foo Bar" # The name of the game
```

## game.toml
This file is used to store information about the game. It is used to store where the game's files are, what the ideal working directory is, and what the game's name is.

```toml
[game]
path = "." # A path relative to the directory containing the game.toml file
working_dir = "." # The ideal working directory, relative to the game's path
texture_path = "assets/textures" # The path to the game's textures, relative to the game's path
name = "My Game" # The name of the game
```

## palette.toml
This stores the block definitions for the editor. It holds properties and how to deal with them, and should ideally reflect what the game internally uses, or atleast be sensible.

```toml
[<id>]
name = "foo" # The name of the block
texturePos = [0, 0] # The position of the block's texture in the texture sheet
metaUse = "None" # See below
metaMin = 0 # The minimum value of the block's meta
metaMax = 65535 # The maximum value of the block's meta
isLiquid = false # Whether the block is a liquid
liquidSource = <id> # The id of the block that is the source of the liquid
liquidFlow = <id> # The id of the block that is the flow of the liquid
```

`metaUse` determines how the meta value is presented in the editor. It can be one of the following:
 - `None`: The meta value is not used, and cannot be changed.
 - `Color`: The meta value should be treated as a color, and will display a color picker.
 - `Direction`: The meta value should be treated as a direction, and will display two dials to select the direction.
    - `DirectionHorz`: Will display ONLY the horizontal dial.
    - `DirectionVert`: Will display ONLY the vertical dial.
 - `Level`: The meta value is a level of some kind, and will be displayed as a slider.
 - `Toggle`: The meta value is a boolean. It will be displayed as a checkbox.
 - `BitField`: The meta value is a bitfield. It will be displayed as 16 checkboxes that control each bit.
 - `OtherBlock`: The meta value is another block id. It will be displayed as a combo box containing every block id.

## entitypalette.toml
The entity palette stores some information about the entities that can be placed in the world. It provides the entity name, some legal metadata, a visual type and a texture.

```toml
[<id>]
name = "foo" # The name of the entity, used for i/o
visualType = "sprite" # See below
texture = "foo.png" # The path to the entity's texture, relative to the texture path

# This next section allows you to specify arbitrary metadata for the entity.
[<id>.properties]
"keyName" = [
    <type>, # The type of the metadata, see below
    description = "foo" # A description of the metadata
    <min>, # The minimum value of the metadata
    <max>, # The maximum value of the metadata
    <default> # The default value of the metadata
]
```

`visualType` may be one of the following:
 - `sprite`: The entity will be displayed as a sprite.
 - `model`: The entity will be displayed as a model. An additional `model` property must be specified.

Entity metadata type may be one of the following:
 - `INT`: The metadata is an integer.
 - `FLOAT`: The metadata is a float.
 - `STRING`: The metadata is a string.
 - `BOOL`: The metadata is a boolean.
 - `LIST`: The metadata is a list. If it is a list, instead of a `min` value, you specify the list of values. Instead of `max`, you specify the default value.