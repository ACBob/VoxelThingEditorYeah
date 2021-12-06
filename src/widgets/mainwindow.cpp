#include "mainwindow.hpp"

#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

#include <QMessageBox>

#include "editor4pane.hpp"
#include "settingsdialog.hpp"

#include "../editor/blockdefs.hpp"
#include "../editor/tools.hpp"
#include "../world/chunk.hpp"
#include "../world/world.hpp"

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
	this->setWindowTitle( tr( "VoxelThingEditorYeah" ) );
	this->setMinimumSize( 800, 600 );

	m_blockDefs = LoadBlockDefs( ":/example/palette_internal.toml" );

	// Main window has toolbars to the left, right and top
	// And the central widget is a 4-way split

	// This toolbar holds the... well, tools
	QToolBar *toolsBar = new QToolBar( "Tools Bar", this );
	this->addToolBar( Qt::LeftToolBarArea, toolsBar );
	toolsBar->setIconSize( QSize( 64, 64 ) );

	QActionGroup *toolGroup = new QActionGroup( this );
	toolGroup->setExclusive( true );

	CTool *handTool		= new CHandTool( this );
	QAction *handAction = toolsBar->addAction( QIcon( ":/img/tool_hand.png" ), handTool->getName() );
	handAction->setCheckable( true );
	handAction->setChecked( true );
	handAction->setActionGroup( toolGroup );
	this->m_tools.push_back( handTool );

	CTool *wrenchTool	  = new CWrenchTool( this );
	QAction *wrenchAction = toolsBar->addAction( QIcon( ":/img/tool_wrench.png" ), wrenchTool->getName() );
	wrenchAction->setCheckable( true );
	wrenchAction->setActionGroup( toolGroup );
	this->m_tools.push_back( wrenchTool );

	connect( toolGroup, SIGNAL( triggered( QAction * ) ), this, SLOT( toolChanged( QAction * ) ) );

	// To the right side of the screen we have a list of chunks
	QDockWidget *thingsDock = new QDockWidget( tr( "Chunks" ), this );
	thingsDock->setAllowedAreas( Qt::RightDockWidgetArea );
	this->addDockWidget( Qt::RightDockWidgetArea, thingsDock );

	QListWidget *thingsList = new QListWidget( thingsDock );
	thingsDock->setWidget( thingsList );

	m_world.createChunk(0, 0, 0);
	m_world.createChunk(1, 0, 0);
	m_world.createChunk(-1, 0, 0);
	m_world.createChunk(0, 1, 0);
	m_world.createChunk(0, -1, 0);
	m_world.createChunk(0, 0, 1);
	m_world.createChunk(0, 0, -1);

	// when a different chunk is selected, we need to update the editor
	connect( thingsList, SIGNAL( currentRowChanged( int ) ), this, SLOT( onChunkSelected( int ) ) );

	// Editor 4-pane
	m_editor = new Editor4Pane( this );
	this->setCentralWidget( m_editor );
	m_editor->setTool( handTool );
	m_editor->setWorld( &m_world );

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

	// Help menu
	QMenu *helpMenu = menuBar->addMenu( tr( "&Help" ) );
	helpMenu->addAction( tr( "&About" ), this, SLOT( showAbout() ) );
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
			m_editor->setTool( tool );
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