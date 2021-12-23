#include "mainwindow.hpp"

#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QMessageBox>
#include <QUndoStack>

#include "ui/dialogs/blocklist.hpp"
#include "ui/dialogs/settingsdialog.hpp"
#include "ui/dialogs/properties.hpp"
#include "ui/docks/currentblock.hpp"
#include "ui/docks/editor4pane.hpp"

#include "editor/defs.hpp"
#include "editor/editorstate.hpp"
#include "editor/tools.hpp"
#include "world/chunk.hpp"
#include "world/loading/worldformat.hpp"
#include "world/world.hpp"

#define addTool(toolname, classname) \
	CTool *toolname = new classname(editorState, this); \
	QAction *toolname##Action = toolsBar->addAction( QIcon(":/img/tool_" #toolname ".png"), toolname->getName() ); \
	toolname##Action->setCheckable(true); \
	toolname##Action->setActionGroup(toolGroup); \
	m_tools.push_back(toolname)

MainWindow::MainWindow( EditorState *editorState, QWidget *parent )
	: QMainWindow( parent ), m_world( editorState, this )
{
	this->setWindowTitle( tr( "VoxelThingEditorYeah" ) );
	this->setMinimumSize( 800, 600 );

	m_editorState			 = editorState;
	m_editorState->m_pWorld	 = &m_world;

	// Worlds always have the 0,0,0 chunk, it cannot be removed
	m_world.createChunk( 0, 0, 0 );

	// Main window has toolbars to the left, right and top
	// And the central widget is a 4-way split

	// This toolbar holds the... well, tools
	QToolBar *toolsBar = new QToolBar( "Tools Bar", this );
	this->addToolBar( Qt::LeftToolBarArea, toolsBar );
	toolsBar->setIconSize( QSize( 48, 48 ) );

	QActionGroup *toolGroup = new QActionGroup( this );
	toolGroup->setExclusive( true );

	addTool(hand, CHandTool);
	handAction->setChecked(true);
	addTool(wrench, CWrenchTool);
	addTool(simulate, CSimulateTool);
	addTool(select, CSelectTool);
	
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
	m_editorState->m_pTool = m_tools[0];

	// Menubar
	QMenuBar *menuBar = new QMenuBar( this );
	this->setMenuBar( menuBar );

	// File menu
	QMenu *fileMenu = menuBar->addMenu( tr( "&File" ) );
	QAction *newFileAction = fileMenu->addAction( tr( "&New" ), this, SLOT( newFile() ), QKeySequence::New );
	newFileAction->setIcon( QIcon::fromTheme( "document-new" ) );

	QAction *openFileAction = fileMenu->addAction( tr( "&Open" ), this, SLOT( openFile() ), QKeySequence::Open );
	openFileAction->setIcon( QIcon::fromTheme( "document-open" ) );

	QAction *saveFileAction = fileMenu->addAction( tr( "&Save" ), this, SLOT( saveFile() ), QKeySequence::Save );
	saveFileAction->setIcon( QIcon::fromTheme( "document-save" ) );

	QAction *saveAsFileAction = fileMenu->addAction( tr( "Save &As" ), this, SLOT( saveAsFile() ), QKeySequence::SaveAs );
	saveAsFileAction->setIcon( QIcon::fromTheme( "document-save-as" ) );

	// Edit menu
	QMenu *editMenu = menuBar->addMenu( tr( "&Edit" ) );

	QAction *undoAction = m_editorState->m_pUndoStack->createUndoAction( this, tr( "Undo" ) );
	undoAction->setIcon( QIcon::fromTheme( "edit-undo" ) );
	undoAction->setShortcut( QKeySequence::Undo );
	editMenu->addAction( undoAction );

	QAction *redoAction = m_editorState->m_pUndoStack->createRedoAction( this, tr( "Redo" ) );
	redoAction->setIcon( QIcon::fromTheme( "edit-redo" ) );
	redoAction->setShortcut( QKeySequence::Redo );
	editMenu->addAction( redoAction );

	editMenu->addSeparator();

	QAction *editWorldPropertiesAction = editMenu->addAction( tr( "Edit World Properties" ), this, SLOT( editWorldProperties() ) );
	editWorldPropertiesAction->setIcon( QIcon::fromTheme( "document-properties" ) );

	editMenu->addSeparator();

	QAction *preferencesAction = editMenu->addAction( tr( "&Preferences" ), this, SLOT( editPreferences() ) );
	preferencesAction->setIcon( QIcon::fromTheme( "preferences-system" ) );


	// View menu
	QMenu *viewMenu = menuBar->addMenu( tr( "&View" ) );
	// Toggle display of chunk list
	viewMenu->addAction( thingsDock->toggleViewAction() );
	viewMenu->addAction( m_editor->toggleViewAction() );
	viewMenu->addAction( toolsBar->toggleViewAction() );
	viewMenu->addAction( currentBlock->toggleViewAction() );

	// Help menu
	QMenu *helpMenu = menuBar->addMenu( tr( "&Help" ) );
	QAction *aboutAction = helpMenu->addAction( tr( "&About" ), this, SLOT( showAbout() ) );
	aboutAction->setIcon( QIcon::fromTheme( "help-about" ) );

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
	actionsBar->addAction( undoAction );
	actionsBar->addAction( redoAction );
}

MainWindow::~MainWindow() {}

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
			m_editorState->setTool( tool );
			return;
		}
	}
}

void MainWindow::editPreferences()
{
	// Show the preferences dialog
	SettingsDialog *dialog = new SettingsDialog( m_editorState, this );
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

		qDebug() << "Chosen block: " << m_editorState->m_nChosenBlockType;
	}
}

void MainWindow::editWorldProperties()
{
	EntityDef def;
	def.properties = {
		{
			"author.name", EntityProperty{
				"Author Name", "STRING", "", "",
				"The name of the author of the world"
			}
		},
		{
			"author.email", EntityProperty{
				"Author Email", "STRING", "", "",
				"The email of the author of the world"
			}
		},
		{
			"author.url", EntityProperty{
				"Author Website", "STRING", "", "",
				"The website of the author of the world"
			}
		},
		{
			"author.license", EntityProperty{
				"World License", "STRING", "CC-0", "CC-0",
				"The license of the world"
			}
		},
		{
			"author.description", EntityProperty{
				"Author Description", "STRING", "Created in VTEY v" VTEY_VERSION, "Created in VTEY v" VTEY_VERSION,
				"An author-provided description of the world"
			}
		}
	};
	def.entityType = "world";
	def.name = m_world.getName();
	
	EntityPropertyDialog *dialog = new EntityPropertyDialog( m_editorState, def, this );

	if ( dialog->exec() == QDialog::Accepted )
	{
		qDebug() << "World properties changed";
	}
}