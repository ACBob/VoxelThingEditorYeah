// -- block definitions
// Handles block definitions

#include <QMap>
#include <QPoint>
#include <QString>

// VTEY_VERSION definition
#include "config.h"

#pragma once

enum BlockMetaType {
	META_NONE,			  // Meta may be written into raw
	META_COLOR,			  // Meta is displayed as a color picker
	META_DIRECTION_HORIZ, // Meta is displayed as a QDial
	META_DIRECTION_VERT,  // Meta is displayed as a QDial
	META_DIRECTION,		  // Meta is displayed as two QDials
	META_LEVEL,			  // Meta is displayed as a slider
	META_TOGGLE,		  // Meta is displayed as a checkbox
	META_BITFIELD,		  // Meta is displayed as 16 checkboxes
	META_OTHERBLOCK		  // Meta is displayed the same as ID
};

struct BlockDef
{
	BlockMetaType metaType;
	QString name;
	QPoint texturePos;
	uint16_t metaMin;
	uint16_t metaMax;
	bool isLiquid;
	uint16_t liquidSource;
	uint16_t liquidFlow;
};

using BlockDefs = QMap<int, BlockDef>;

struct EntityProperty
{
	QString name; // The name of the property
	QString friendlyName; // Name displayed in the editor
	QString type; // The type of the property
	QString defaultValue; // The default value of the property
	QString description; // The description of the property
	QString min; // The minimum value of the property
	QString max; // The maximum value of the property
};

struct EntityDef
{
	QString entityType; // The type of the entity
	QString friendlyName; // The friendly name of the entity
	QString documentation; // Markdown documentation of the entity

	QString visualType; // The type of the visual
	QString texture; // The texture of the visual
	
	QMap<QString, EntityProperty> properties; // The properties of the entity
	QStringList flags; // Flag value is 1 << index
};

using EntityDefs = QMap<QString, EntityDef>;

struct GameDef
{
	QString id;
	QString name;
	QString absolutePath;
	QString workingPath;
	QString texturePath;
	QString filePath;

	BlockDefs *blockDefs;
	EntityDefs *entityDefs;
};

using GameDefs = QMap<QString, GameDef>;

namespace definitions {

	BlockDefs *LoadBlockDefs( const QString &path );
	EntityDefs *LoadEntityDefs( const QString &path );

	void LoadGameDefs( const QString &path, GameDefs &toPutInto );
	void LoadGameDef( const QString &path, GameDef &def );

}