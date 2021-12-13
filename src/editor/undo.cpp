#include "undo.hpp"

#include "editorstate.hpp"

#include "world/world.hpp"
#include "world/chunk.hpp"

// Undo Block Edit
// Undoes/Redoes a change to one block in the world

UndoBlockEdit::UndoBlockEdit( EditorState *editorState, int x, int y, int z, uint32_t oldVoxel, uint32_t newVoxel )
{
    m_editorState = editorState;
    m_x = x;
    m_y = y;
    m_z = z;
    m_oldVoxel = oldVoxel;
    m_newVoxel = newVoxel;
}

void UndoBlockEdit::undo()
{
    m_editorState->m_pWorld->set( m_x, m_y, m_z, m_oldVoxel );
}

void UndoBlockEdit::redo()
{
    m_editorState->m_pWorld->set( m_x, m_y, m_z, m_newVoxel );
}


// Undo Chunk Edit
// Undoes/Redoes a change to every block in a chunk

UndoChunkEdit::UndoChunkEdit( EditorState *editorState, CChunk *chunk, uint32_t *oldVoxels, uint32_t *newVoxels ) {
    m_editorState = editorState;

    m_chunk = chunk;
    m_oldVoxels = oldVoxels;
    m_newVoxels = newVoxels;

    m_oldVoxelsSize = m_chunk->getSize().x * m_chunk->getSize().y * m_chunk->getSize().z;
    m_newVoxelsSize = m_oldVoxelsSize;
}

UndoChunkEdit::~UndoChunkEdit()
{
    delete[] m_oldVoxels;
    delete[] m_newVoxels;
}

void UndoChunkEdit::undo()
{
    for( int i = 0; i < m_oldVoxelsSize; i++ )
    {
        m_chunk->set( i, m_oldVoxels[i] );
    }

    m_chunk->rebuildModel();
}

void UndoChunkEdit::redo()
{
    for( int i = 0; i < m_newVoxelsSize; i++ )
    {
        m_chunk->set( i, m_newVoxels[i] );
    }

    m_chunk->rebuildModel();
}