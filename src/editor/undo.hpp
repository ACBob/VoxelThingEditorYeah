// -- undo/redo
// Holds the Undo/Redo commands that we use

#include <QUndoCommand>

class EditorState;
class CChunk;

// Undoes a change to a single block
class UndoBlockEdit : public QUndoCommand
{
    public:
    UndoBlockEdit( EditorState *editorState, int x, int y, int z, uint32_t oldVoxel, uint32_t newVoxel );

    void undo() override;
    void redo() override;

    private:
    EditorState *m_editorState;
    int m_x;
    int m_y;
    int m_z;
    uint32_t m_newVoxel;
    uint32_t m_oldVoxel;
};

// Undoes a change to an entire chunk
class UndoChunkEdit : public QUndoCommand
{
    public:
    UndoChunkEdit( EditorState *editorState, CChunk *chunk, uint32_t *oldVoxels, uint32_t *newVoxels );
    ~UndoChunkEdit();

    void undo() override;
    void redo() override;

    private:
    EditorState *m_editorState;
    CChunk *m_chunk;
    uint32_t *m_oldVoxels;
    uint32_t *m_newVoxels;
    int m_oldVoxelsSize;
    int m_newVoxelsSize;
};