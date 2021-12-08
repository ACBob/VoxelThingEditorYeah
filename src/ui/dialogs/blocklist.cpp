#include "blocklist.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include "editor/editorstate.hpp"
#include "ui/widgets/blocktexture.hpp"

BlockList::BlockList( EditorState *editorState, QWidget *parent ) :
    QDialog( parent )
{
    m_editorState = editorState;

    setWindowTitle( tr( "Block List" ) );
    setWindowIcon( QIcon( ":/img/icon_blocklist.png" ) );

    QVBoxLayout *layout = new QVBoxLayout( this );
    this->setLayout( layout );

    m_list = new QListWidget( this );
    layout->addWidget( m_list );

    for ( int i = 0; i < m_editorState->blockDefs->size(); i++ )
    {
        BlockDef blockDef = m_editorState->blockDefs->value( i );
        QString name = QString( "%1 : %2" ).arg( QString::number(i), blockDef.name );
        QListWidgetItem *item = new QListWidgetItem( name, m_list );
        item->setData( Qt::UserRole, i );
        
        m_list->addItem( item );
    }

    connect( m_list, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( onItemClicked( QListWidgetItem * ) ) );
    connect( m_list, SIGNAL( itemSelectionChanged() ), this, SLOT( onItemSelectionChanged() ) );

    m_blockPreview = new BlockTexture( m_editorState, m_editorState->chosenBlockType, this );
    layout->addWidget( m_blockPreview );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    layout->addLayout( buttonLayout );

    QPushButton *okButton = new QPushButton( tr( "OK" ), this );
    buttonLayout->addWidget( okButton );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    QPushButton *cancelButton = new QPushButton( tr( "Cancel" ), this );
    buttonLayout->addWidget( cancelButton );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    m_selectedBlock = m_editorState->chosenBlockType;
    m_list->setCurrentRow( m_selectedBlock );
}

BlockList::~BlockList()
{
}

uint16_t BlockList::getSelectedBlock()
{
    return m_selectedBlock;
}

void BlockList::onItemClicked( QListWidgetItem *item )
{
    m_selectedBlock = item->data( Qt::UserRole ).toInt();
    m_blockPreview->setBlockId( m_selectedBlock );
}

void BlockList::onItemSelectionChanged()
{
    onItemClicked( m_list->currentItem() );
}