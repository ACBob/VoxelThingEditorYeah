// -- Block List
// Displays a list of blocks and allows the user to select one.
// Each block is represented by its texture and its name.

#include <QDialog>

class QListWidget;
class EditorState;

class BlockList : public QDialog
{
    Q_OBJECT

    public:
        BlockList(EditorState *editorState, QWidget *parent = 0);
        ~BlockList();

        uint16_t getSelectedBlock();

    private:
        EditorState *editorState;
        QListWidget *list;

    private slots:
        void onItemSelected(QListWidgetItem *item);
};