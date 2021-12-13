// -- Editor State
// Holds the state of the editor, something needed by just about everything.

#include <QObject>
#include <QString>

#include "world/vector.hpp"

#pragma once

class CWorld;
class CTool;
class QUndoStack;

// TODO: any way to forward-declare a using statement?
#include "defs.hpp"

class EditorState : public QObject
{
	Q_OBJECT

  public:
	EditorState( QObject *parent = 0 );

	// The current filename.
	QString m_sFilename;

	// Pointer to the world object.
	CWorld *m_pWorld;

	// Pointer to the current tool.
	CTool *m_pTool;

	// Pointer to a QVector of block definitions.
	BlockDefs *m_pBlockDefs;

	// Pointer to the games we know about.
	GameDefs m_gameDefs;

	// Pointer to the current game.
	GameDef m_game;

	// TODO: Pointer to a QVector of Entity definitions.
	// EntityDefs *entityDefs;

	// Holds the block type / meta we want to place.
	// Interpreted by tools differently.
	uint16_t m_nChosenBlockType;
	uint16_t m_nChosenBlockMeta;

	// The absolute path of the block texture.
	QString m_sBlockTexturePath;

    // The undo stack
    QUndoStack *m_pUndoStack;

    // Selection Area
    Vector3i m_vecSelectionAreaStart;
    Vector3i m_vecSelectionAreaEnd;

	// Functions to set
	// These ideally should be what you call to set stuff, where-as you just access the variables directly.
	// These functions notifies the editor that something has changed, and the editor should update.
	void setFilename( QString filename );
	void setWorld( CWorld *world );
	void setTool( CTool *tool );
	void setBlockDefs( BlockDefs *blockDefs );
	void setChosenBlockType( uint16_t chosenBlockType );
	void setChosenBlockMeta( uint16_t chosenBlockMeta );
	void setBlockTexturePath( QString blockTexturePath );
	void setGame( GameDef game );

  signals:
	void filenameChanged( QString filename );
	void worldChanged( CWorld *world );
	void toolChanged( CTool *tool );
	void blockDefsChanged( BlockDefs *blockDefs );
	void chosenBlockTypeChanged( uint16_t chosenBlockType );
	void chosenBlockMetaChanged( uint16_t chosenBlockMeta );
	void blockTexturePathChanged( QString blockTexturePath );
	void gameDefsChanged( GameDefs *gameDefs );
	void gameChanged( GameDef *game );
};