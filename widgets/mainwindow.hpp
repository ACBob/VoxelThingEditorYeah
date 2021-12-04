#include <QMainWindow>

class Editor4Pane; // forward declaration
class CChunk; // forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:

        MainWindow(QWidget *parent);
        ~MainWindow();

    public slots:

        // void newFile();
        // void openFile();
        // void saveFile();
        // void saveAsFile();
        // void closeFile();
        // void closeAllFiles();
        // void closeAllFilesButThis();

        void onChunkSelected(int);

    private:
        Editor4Pane *m_editor;

        std::vector<CChunk *> m_chunks;
};