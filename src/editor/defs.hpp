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

struct GameDef
{
	QString id;
	QString name;
	QString absolutePath;
	QString workingPath;
	QString texturePath;
	QString filePath;

	BlockDefs *blockDefs;
};

using GameDefs = QMap<QString, GameDef>;

namespace definitions {

	BlockDefs *LoadBlockDefs( const QString &path );

	void LoadGameDefs( const QString &path, GameDefs &toPutInto );
	void LoadGameDef( const QString &path, GameDef &def );

}

struct EntityProperty
{
	QString name;
	QString type;
	QString value;
	QString defaultValue;
	QString description;
};

struct EntityDef
{
	QString name;
	QString entityType;
	
	QMap<QString, EntityProperty> properties;
};