#include "currentblock.hpp"

#include "ui/widgets/blocktexture.hpp"

#include "editor/editorstate.hpp"

#include <QVBoxLayout>
#include <QLabel>

CurrentBlock::CurrentBlock( EditorState *editorState, QWidget *parent ) : QDockWidget( parent )
{
    m_editorState = editorState;

    setWindowTitle( tr( "Current Block" ) );

    QWidget *widget = new QWidget( this );
    setWidget( widget );

    QVBoxLayout *layout = new QVBoxLayout(widget);
    widget->setLayout( layout );

    QLabel *label = new QLabel( tr( "Current Block" ), widget );
    layout->addWidget( label );
    m_blockTexWidget = new BlockTexture( m_editorState, m_editorState->chosenBlockType, widget );
    layout->addWidget( m_blockTexWidget );
    m_blockTexWidget->setFixedSize( 64, 64 );

    connect( m_editorState, SIGNAL( chosenBlockTypeChanged( uint16_t ) ), this, SLOT( updateCurrentBlock( uint16_t ) ) );
}

CurrentBlock::~CurrentBlock()
{

}

void CurrentBlock::updateCurrentBlock( uint16_t blockType )
{
    m_blockTexWidget->setBlockId( blockType );
}