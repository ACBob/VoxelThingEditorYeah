// -- Current Block dock
// Shows the current block ID, texture and meta data

#include <QDockWidget>

class EditorState;
class BlockTexture;

class CurrentBlock : public QDockWidget
{
    Q_OBJECT

    public:
        CurrentBlock(EditorState *editorState, QWidget *parent = 0);
        ~CurrentBlock();

    private slots:
        void updateCurrentBlock(uint16_t blockId);

        void browse();

    private:
        EditorState *m_editorState;
        uint16_t m_BlockID;

        BlockTexture *m_blockTexWidget;
};
        