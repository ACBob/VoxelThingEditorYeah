#include "mainwindow.hpp"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QListWidget>

#include "editor4pane.hpp"

#include "../world/chunk.hpp"
#include "../editor/tools.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	this->setWindowTitle(tr("VoxelThingEditorYeah"));
	this->setMinimumSize(800, 600);

	// Main window has toolbars to the left, right and top
	// And the central widget is a 4-way split

	// This toolbar holds the... well, tools
	QToolBar *toolsBar = new QToolBar(this);
	this->addToolBar(Qt::LeftToolBarArea, toolsBar);
	toolsBar->setIconSize(QSize(64, 64));

	QActionGroup *toolGroup = new QActionGroup(this);
	toolGroup->setExclusive(true);

	CTool *handTool = new CHandTool(this);
	QAction *handAction = toolsBar->addAction( QIcon(":/img/tool_hand.png"), handTool->getName() );
	handAction->setCheckable(true);
	handAction->setChecked(true);
	handAction->setActionGroup(toolGroup);
	this->m_tools.push_back(handTool);

	connect( toolGroup, SIGNAL(triggered(QAction*)), this, SLOT(toolChanged(QAction*)) );

	// To the right side of the screen we have a list of chunks
	QDockWidget *thingsDock = new QDockWidget(tr("Chunks"), this);
	thingsDock->setAllowedAreas(Qt::RightDockWidgetArea);
	this->addDockWidget(Qt::RightDockWidgetArea, thingsDock);

	QListWidget *thingsList = new QListWidget(thingsDock);
	thingsDock->setWidget(thingsList);

	for (int i = 0; i < 5; i++)
	{
		thingsList->addItem(tr("Chunk %1").arg(i));
		m_chunks.push_back(
			new CChunk(i, 0, 0)
		);
	}

	// when a different chunk is selected, we need to update the editor
	connect(thingsList, SIGNAL(currentRowChanged(int)), this, SLOT(onChunkSelected(int)));

	// Menubar
	QMenuBar *menuBar = new QMenuBar(this);
	this->setMenuBar(menuBar);

	// File menu
	QMenu *fileMenu = menuBar->addMenu(tr("&File"));
	fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
	fileMenu->addAction(tr("&Open"), this, SLOT(openFile()), QKeySequence::Open);
	fileMenu->addAction(tr("&Save"), this, SLOT(saveFile()), QKeySequence::Save);

	// Edit menu
	QMenu *editMenu = menuBar->addMenu(tr("&Edit"));

	// View menu
	QMenu *viewMenu = menuBar->addMenu(tr("&View"));

	// Help menu
	QMenu *helpMenu = menuBar->addMenu(tr("&Help"));

	// Editor 4-pane
	m_editor = new Editor4Pane(this);
	this->setCentralWidget(m_editor);

	m_editor->setTool(handTool);
}

MainWindow::~MainWindow()
{
	for (CChunk* chunk : m_chunks)
	{
		delete chunk;
	}
}

void MainWindow::onChunkSelected(int index)
{
	// update the editor
	CChunk *chunk = m_chunks[index];
	m_editor->setChunk(chunk);
}

void MainWindow::toolChanged(QAction *action)
{
	// find the tool that was selected
	for (CTool *tool : m_tools)
	{
		if (tool->getName() == action->text())
		{
			m_editor->setTool(tool);
			return;
		}
	}
}