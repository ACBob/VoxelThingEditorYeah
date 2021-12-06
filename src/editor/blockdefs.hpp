// -- block definitions
// Handles block definitions

#include <QMap>
#include <QPoint>
#include <QString>

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
};

using BlockDefs = QMap<int, BlockDef>;

BlockDefs LoadBlockDefs( const QString &path );