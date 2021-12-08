#include <QMainWindow>

#include "editor/defs.hpp"

#include "world/world.hpp"
#include <QVector>

class Editor4Pane; // forward declaration
class CChunk;	   // forward declaration
class CTool;	   // forward declaration
class EditorState; // forward declaration

class MainWindow : public QMainWindow
{
	Q_OBJECT;

  public:
	MainWindow( EditorState *editorState, QWidget *parent );
	~MainWindow();

  private slots:

	// void newFile();
	void openFile();
	void saveFile();
	// void saveAsFile();
	// void closeFile();
	// void closeAllFiles();
	// void closeAllFilesButThis();

	void onChunkSelected( int );
	void toolChanged( QAction * );

	void editPreferences();

	void showAbout();

	void showBlocks();

  private:
	Editor4Pane *m_editor;
	CWorld m_world;
	QVector<CTool *> m_tools;

	EditorState *m_editorState;
};