#include "editor4pane.hpp"

#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>

Editor4Pane::Editor4Pane(QWidget *parent) : QDockWidget(parent)
{
    this->setWindowTitle("Editor4Pane");

    QWidget *centralWidget = new QWidget(this);
    this->setWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

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

    // Disable all features
    // UITODO: At some point, this will be floatable, closable, etc.
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
}
