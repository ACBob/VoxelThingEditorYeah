#include "mainwindow.hpp"

#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

#include <QMessageBox>
#include <QFileDialog>
#include <QUndoStack>

#include "ui/docks/editor4pane.hpp"
#include "ui/docks/currentblock.hpp"
#include "ui/dialogs/settingsdialog.hpp"
#include "ui/dialogs/blocklist.hpp"

#include "editor/blockdefs.hpp"
#include "editor/tools.hpp"
#include "editor/editorstate.hpp"
#include "world/chunk.hpp"
#include "world/world.hpp"
#include "world/loading/worldformat.hpp"

MainWindow::MainWindow( EditorState *editorState, QWidget *parent ) : QMainWindow( parent ),
	m_world( editorState, this )
{
	this->setWindowTitle( tr( "VoxelThingEditorYeah" ) );
	this->setMinimumSize( 800, 600 );

	m_editorState = editorState;
	m_editorState->blockDefs = LoadBlockDefs( ":/example/palette_internal.toml");
	m_editorState->world = &m_world;

	// Worlds always have the 0,0,0 chunk, it cannot be removed
	m_world.createChunk( 0, 0, 0 );

	// Main window has toolbars to the left, right and top
	// And the central widget is a 4-way split

	// This toolbar holds the... well, tools
	QToolBar *toolsBar = new QToolBar( "Tools Bar", this );
	this->addToolBar( Qt::LeftToolBarArea, toolsBar );
	toolsBar->setIconSize( QSize( 64, 64 ) );

	QActionGroup *toolGroup = new QActionGroup( this );
	toolGroup->setExclusive( true );

	CTool *handTool		= new CHandTool( editorState, this );
	QAction *handAction = toolsBar->addAction( QIcon( ":/img/tool_hand.png" ), handTool->getName() );
	handAction->setCheckable( true );
	handAction->setChecked( true );
	handAction->setActionGroup( toolGroup );
	this->m_tools.push_back( handTool );

	CTool *wrenchTool	  = new CWrenchTool( editorState, this );
	QAction *wrenchAction = toolsBar->addAction( QIcon( ":/img/tool_wrench.png" ), wrenchTool->getName() );
	wrenchAction->setCheckable( true );
	wrenchAction->setActionGroup( toolGroup );
	this->m_tools.push_back( wrenchTool );

	CTool *simulateTool	  = new CSimulateTool( editorState, this );
	QAction *simulateAction = toolsBar->addAction( QIcon( ":/img/tool_simulate.png" ), simulateTool->getName() );
	simulateAction->setCheckable( true );
	simulateAction->setActionGroup( toolGroup );
	this->m_tools.push_back( simulateTool );

	connect( toolGroup, SIGNAL( triggered( QAction * ) ), this, SLOT( toolChanged( QAction * ) ) );

	// To the right side of the screen we have a list of chunks
	QDockWidget *thingsDock = new QDockWidget( tr( "Chunks" ), this );
	thingsDock->setAllowedAreas( Qt::RightDockWidgetArea );
	this->addDockWidget( Qt::RightDockWidgetArea, thingsDock );
	thingsDock->setVisible( false ); // hide by default

	QListWidget *thingsList = new QListWidget( thingsDock );
	thingsDock->setWidget( thingsList );

	// when a different chunk is selected, we need to update the editor
	connect( thingsList, SIGNAL( currentRowChanged( int ) ), this, SLOT( onChunkSelected( int ) ) );

	// Current chosen block
	CurrentBlock *currentBlock = new CurrentBlock( editorState, this );
	this->addDockWidget( Qt::RightDockWidgetArea, currentBlock );
	currentBlock->setVisible( true );

	// Editor 4-pane
	m_editor = new Editor4Pane( editorState, this );
	this->setCentralWidget( m_editor );
	m_editorState->tool = m_tools[0];

	// Menubar
	QMenuBar *menuBar = new QMenuBar( this );
	this->setMenuBar( menuBar );

	// File menu
	QMenu *fileMenu = menuBar->addMenu( tr( "&File" ) );
	fileMenu->addAction( tr( "&New" ), this, SLOT( newFile() ), QKeySequence::New );
	fileMenu->addAction( tr( "&Open" ), this, SLOT( openFile() ), QKeySequence::Open );
	fileMenu->addAction( tr( "&Save" ), this, SLOT( saveFile() ), QKeySequence::Save );

	// Edit menu
	QMenu *editMenu = menuBar->addMenu( tr( "&Edit" ) );
	editMenu->addAction( tr( "&Preferences" ), this, SLOT( editPreferences() ), QKeySequence::Preferences );

	// View menu
	QMenu *viewMenu = menuBar->addMenu( tr( "&View" ) );
	// Toggle display of chunk list
	viewMenu->addAction( thingsDock->toggleViewAction() );
	viewMenu->addAction( m_editor->toggleViewAction() );
	viewMenu->addAction( toolsBar->toggleViewAction() );
	viewMenu->addAction( currentBlock->toggleViewAction() );

	// Help menu
	QMenu *helpMenu = menuBar->addMenu( tr( "&Help" ) );
	helpMenu->addAction( tr( "&About" ), this, SLOT( showAbout() ) );

	// The actions bar
	// Holds various icons and buttons
	QToolBar *actionsBar = new QToolBar( "Actions Bar", this );
	this->addToolBar( Qt::TopToolBarArea, actionsBar );
	actionsBar->setIconSize( QSize( 32, 32 ) );

	// The show block list action
	// When pushed it opens the block list dialog
	QAction *showBlocksAction = actionsBar->addAction( QIcon( ":/img/icon_blocklist.png" ), tr( "Show Blocks" ) );
	connect( showBlocksAction, SIGNAL( triggered() ), this, SLOT( showBlocks() ) );

	// Single chunk mode action
	// When pushed it toggles the single chunk mode
	QAction *singleChunkAction = actionsBar->addAction( QIcon( ":/img/icon_chunkview.png" ), tr( "Single Chunk" ) );
	singleChunkAction->setCheckable( true );
	singleChunkAction->setChecked( false );

	// Undo/redo actions
	actionsBar->addSeparator();
	actionsBar->addAction( m_editorState->undoStack->createUndoAction( actionsBar, tr( "Undo" ) ) );
	actionsBar->addAction( m_editorState->undoStack->createRedoAction( actionsBar, tr( "Redo" ) ) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::onChunkSelected( int index )
{
	// update the editor
	// CChunk *chunk = m_chunks[index];
	// m_editor->setChunk( chunk );
}

void MainWindow::toolChanged( QAction *action )
{
	// find the tool that was selected
	for ( CTool *tool : m_tools )
	{
		if ( tool->getName() == action->text() )
		{
			m_editorState->tool = tool;
			return;
		}
	}
}

void MainWindow::editPreferences()
{
	// Show the preferences dialog
	SettingsDialog *dialog = new SettingsDialog( this );
	dialog->exec();
}

void MainWindow::showAbout()
{
	QMessageBox::about( this, "VoxelThingEditorYeah", "VoxelEditorThingYeah - The VoxelThingYeah Editor." );
}

void MainWindow::openFile()
{
	// Display a dialog to select only one directory
	QFileDialog dialog( this );
	dialog.setFileMode( QFileDialog::Directory );
	dialog.setOption( QFileDialog::ShowDirsOnly, true );
	dialog.setOption( QFileDialog::DontUseNativeDialog, true );
	dialog.setWindowTitle( tr( "Open Directory" ) );
	dialog.setDirectory( QDir::homePath() );
	dialog.setLabelText( QFileDialog::Accept, tr( "Open" ) );

	if ( dialog.exec() == QDialog::Accepted )
	{
		QStringList files = dialog.selectedFiles();

		QString path = files[0];

		qDebug() << "Opening directory: " << path;

		// TODO: multiple format support
		VoxelFormatYeah *format = new VoxelFormatYeah();
		format->Load( &m_world, path );
		delete format;		
	}
}

void MainWindow::saveFile()
{
	// Display a dialog to select only one directory
	QFileDialog dialog( this );
	dialog.setFileMode( QFileDialog::Directory );
	dialog.setOption( QFileDialog::ShowDirsOnly, true );
	dialog.setOption( QFileDialog::DontUseNativeDialog, true );
	dialog.setWindowTitle( tr( "Save Directory" ) );
	dialog.setDirectory( QDir::homePath() );
	dialog.setLabelText( QFileDialog::Accept, tr( "Save" ) );

	if ( dialog.exec() == QDialog::Accepted )
	{
		QStringList files = dialog.selectedFiles();

		QString path = files[0];

		qDebug() << "Saving directory: " << path;

		// TODO: multiple format support
		VoxelFormatYeah *format = new VoxelFormatYeah();
		format->Save( &m_world, path );
		delete format;
	}
}

void MainWindow::showBlocks()
{
	// Show the block list dialog
	BlockList *dialog = new BlockList( m_editorState, this );
	
	if ( dialog->exec() == QDialog::Accepted )
	{
		m_editorState->setChosenBlockType( dialog->getSelectedBlock() );

		qDebug() << "Chosen block: " << m_editorState->chosenBlockType;
	}		
}