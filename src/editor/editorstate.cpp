#include "editorstate.hpp"

#include <QUndoStack>
#include <QSettings>

EditorState::EditorState( QObject *parent ) : QObject(parent) {
    m_sFilename		 = "";
	m_pWorld			 = nullptr;
	m_pTool			 = nullptr;
	m_nChosenBlockType	 = 1;
	m_nChosenBlockMeta	 = 0;
	m_sBlockTexturePath = ":/img/texatlas_internal.png";

    m_pBlockDefs = nullptr;

	m_pUndoStack = new QUndoStack( this );
	m_pUndoStack->setUndoLimit( 100 );
	m_pUndoStack->clear();

    // Game definition stuff
    QSettings settings;
    definitions::LoadGameDefs( settings.value( "gameDefFileLocation", ":/example/games.toml" ).toString(), m_gameDefs );
    if ( settings.contains( "gameId" ) )
        if (m_gameDefs.contains( settings.value( "gameId" ).toString() ))
            m_game = m_gameDefs.value( settings.value( "gameId" ).toString() );
        else
            m_game = m_gameDefs.begin().value();
    else
        m_game = m_gameDefs.begin().value();
    m_pBlockDefs = m_game.blockDefs;
    m_pEntityDefs = m_game.entityDefs;
}

void EditorState::setFilename( QString filename )
{
    m_sFilename = filename;
    emit filenameChanged( filename );
}
void EditorState::setWorld( CWorld *world )
{
    m_pWorld = world;
    emit worldChanged( world );
}
void EditorState::setTool( CTool *tool )
{
    m_pTool = tool;
    emit toolChanged( tool );
}
void EditorState::setBlockDefs( BlockDefs *blockDefs )
{
    m_pBlockDefs = blockDefs;
    emit blockDefsChanged( blockDefs );
}
void EditorState::setChosenBlockType( uint16_t chosenBlockType )
{
    m_nChosenBlockType = chosenBlockType;
    emit chosenBlockTypeChanged( chosenBlockType );
}
void EditorState::setChosenBlockMeta( uint16_t chosenBlockMeta )
{
    m_nChosenBlockMeta = chosenBlockMeta;
    emit chosenBlockMetaChanged( chosenBlockMeta );
}
void EditorState::setBlockTexturePath( QString blockTexturePath )
{
    m_sBlockTexturePath = blockTexturePath;
    emit blockTexturePathChanged( blockTexturePath );
}
void EditorState::setGame( QString gameId )
{
    if ( !m_gameDefs.contains(gameId) ) {
        qWarning() << "Unknown Game ID " << gameId;
        return;
    }

    m_game = m_gameDefs.value( gameId );
    m_pBlockDefs = m_game.blockDefs;
    emit gameChanged( &m_game );
}