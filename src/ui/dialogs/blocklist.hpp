// -- Block List
// Displays a list of blocks and allows the user to select one.
// Each block is represented by its texture and its name.

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class EditorState;
class BlockTexture;

class BlockList : public QDialog
{
    Q_OBJECT

    public:
        BlockList(EditorState *editorState, QWidget *parent = 0);
        ~BlockList();

        uint16_t getSelectedBlock();

    private:
        EditorState *m_editorState;
        QListWidget *m_list;
        uint16_t m_selectedBlock;

        BlockTexture *m_blockPreview;

    private slots:
        void onItemClicked(QListWidgetItem *item);
        void onItemSelectionChanged();
};