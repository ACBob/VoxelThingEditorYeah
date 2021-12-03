#include "mainwindow.hpp"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDockWidget>

#include "editor4pane.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	this->setWindowTitle(tr("VoxelThingEditorYeah"));
	this->setMinimumSize(800, 600);

	// Main window has toolbars to the left, right and top
	// And the central widget is a 4-way split

	// This toolbar holds the... well, tools
	QToolBar *toolsBar = new QToolBar(this);
	this->addToolBar(Qt::LeftToolBarArea, toolsBar);

	QActionGroup *toolGroup = new QActionGroup(this);
	toolGroup->setExclusive(true);

	// We don't have any tools so here's dummy code
	for (int i = 0; i < 10; i++)
	{
		QAction *action = new QAction(this);
		action->setCheckable(true);
		action->setText(tr("Tool %1").arg(i));
		toolGroup->addAction(action);
		toolsBar->addAction(action);
	}

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
	Editor4Pane *editor = new Editor4Pane(this);
	this->setCentralWidget(editor);
}