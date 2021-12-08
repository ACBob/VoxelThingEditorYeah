#include "currentblock.hpp"

#include "ui/dialogs/blocklist.hpp"
#include "ui/widgets/blocktexture.hpp"

#include "editor/editorstate.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

CurrentBlock::CurrentBlock( EditorState *editorState, QWidget *parent ) : QDockWidget( parent )
{
	m_editorState = editorState;

	setWindowTitle( tr( "Current Block" ) );

	QWidget *widget = new QWidget( this );
	setWidget( widget );

	QGridLayout *layout = new QGridLayout( widget );
	widget->setLayout( layout );

	QLabel *label = new QLabel( tr( "Current Block" ), widget );
	layout->addWidget( label, 0, 0 );
	m_blockTexWidget = new BlockTexture( m_editorState, m_editorState->chosenBlockType, widget );
	layout->addWidget( m_blockTexWidget, 1, 0 );
	m_blockTexWidget->setFixedSize( 64, 64 );

	// Browse/Count buttons
	QVBoxLayout *buttonLayout = new QVBoxLayout();
	layout->addLayout( buttonLayout, 1, 1 );

	QPushButton *browseButton = new QPushButton( tr( "Browse" ), widget );
	buttonLayout->addWidget( browseButton );
	connect( browseButton, SIGNAL( clicked() ), this, SLOT( browse() ) );

	QPushButton *countButton = new QPushButton( tr( "Count" ), widget );
	buttonLayout->addWidget( countButton );
	connect( countButton, SIGNAL( clicked() ), this, SLOT( count() ) );

	connect( m_editorState, SIGNAL( chosenBlockTypeChanged( uint16_t ) ), this,
			 SLOT( updateCurrentBlock( uint16_t ) ) );

	widget->setMaximumHeight( 128 );
}

CurrentBlock::~CurrentBlock() {}

void CurrentBlock::updateCurrentBlock( uint16_t blockType ) { m_blockTexWidget->setBlockId( blockType ); }

void CurrentBlock::browse()
{
	BlockList *blockList = new BlockList( m_editorState, this );

	if ( blockList->exec() == QDialog::Accepted )
	{
		m_editorState->setChosenBlockType( blockList->getSelectedBlock() );
	}
}