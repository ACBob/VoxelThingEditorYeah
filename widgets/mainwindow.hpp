#include <QMainWindow>

#include "../editor/blockdefs.hpp"

class Editor4Pane; // forward declaration
class CChunk; // forward declaration
class CTool; // forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:

        MainWindow(QWidget *parent);
        ~MainWindow();

    private slots:

        // void newFile();
        // void openFile();
        // void saveFile();
        // void saveAsFile();
        // void closeFile();
        // void closeAllFiles();
        // void closeAllFilesButThis();

        void onChunkSelected(int);
        void toolChanged(QAction*);

        void editPreferences();

        void showAbout();

    private:
        Editor4Pane *m_editor;

        std::vector<CChunk *> m_chunks;
        std::vector<CTool *> m_tools;

        BlockDefs m_blockDefs;
};