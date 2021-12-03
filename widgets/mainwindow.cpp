#include "mainwindow.hpp"

#include <QToolBar>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDockWidget>

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

	// Create the central widget that expands to fill the window
	QWidget *centralWidget = new QWidget(this);
	this->setCentralWidget(centralWidget);
	
	// Layout the central widget
	QVBoxLayout *layout = new QVBoxLayout(centralWidget);
	centralWidget->setLayout(layout);

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


	// Split the central widget into left/right
	QSplitter *leftRightSplitter = new QSplitter(Qt::Horizontal, centralWidget);
	leftRightSplitter->setChildrenCollapsible(false);
	layout->addWidget(leftRightSplitter);

	// Split the left/right into top/bottom
	QSplitter *leftUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
	leftUpDownSplitter->setChildrenCollapsible(false);
	
	QSplitter *rightUpDownSplitter = new QSplitter(Qt::Vertical, leftRightSplitter);
	rightUpDownSplitter->setChildrenCollapsible(false);

	// TODO: Synchronize the splitter sizes

	// Put buttons in them for testing
	QPushButton *button = new QPushButton(tr("Top Left"), leftUpDownSplitter);
	QPushButton *button3 = new QPushButton(tr("Bottom Left"), leftUpDownSplitter);
	QPushButton *button2 = new QPushButton(tr("Top Right"), rightUpDownSplitter);
	QPushButton *button4 = new QPushButton(tr("Bottom Right"), rightUpDownSplitter);
}