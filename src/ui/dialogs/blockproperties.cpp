#include "blockproperties.hpp"

// TODO: Setting

#include "ui/widgets/colorpicker.hpp"
#include "ui/widgets/betterslider.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDial>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>

BlockPropertyDialog::BlockPropertyDialog( BlockDefs *defs, uint16_t id, uint16_t meta, QWidget *parent )
	: QDialog( parent )
{
	setWindowTitle( "Block Properties" );

	QVBoxLayout *layout = new QVBoxLayout( this );
	setLayout( layout );

	QPushButton *acceptButton = new QPushButton( tr( "OK" ), this );
	connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

	m_layout = new QFormLayout( this );

	layout->addLayout( m_layout );
	layout->addWidget( acceptButton );

	QLabel *label	 = new QLabel( tr( "ID:" ), this );
	m_idPicker		 = new QComboBox( this );
	m_metaOtherBlock = new QComboBox( this );
	for ( int blockId : defs->keys() )
	{
		BlockDef def = defs->value( blockId );
		m_idPicker->addItem( QString::number( blockId ) + ": " + def.name );
		m_metaOtherBlock->addItem( QString::number( blockId ) + ": " + def.name );
	}

	m_layout->addRow( label, m_idPicker );

	connect( m_idPicker, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onIdSelectionChange( int ) ) );

	connect( m_metaOtherBlock, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onMetaChangeComboBox( int ) ) );

	this->id   = id;
	this->meta = meta;
	this->defs = defs;

	// Setup the appropriate stuff for the metas
	m_metaRaw = new QLineEdit( this );

	m_metaLabel = new QLabel( tr( "Meta:" ), this );

	m_metaColor = new ColorPicker( this );
	m_metaColor->enableAlpha();
	connect( m_metaColor, SIGNAL( colorChanged( QColor ) ), this, SLOT( onMetaChangeColorPicker( QColor ) ) );

	m_metaDirectionHorz = new QDial( this );
	m_metaDirectionHorz->setNotchesVisible( true );
	m_metaDirectionHorz->setWrapping( true );
	m_metaDirectionHorz->setMinimum( 0 );
	m_metaDirectionHorz->setMaximum( 4 );
	connect( m_metaDirectionHorz, SIGNAL( valueChanged( int ) ), this, SLOT( onMetaChangeDial( int ) ) );

	m_metaDirectionVert = new QDial( this );
	m_metaDirectionVert->setNotchesVisible( true );
	m_metaDirectionVert->setWrapping( true );
	m_metaDirectionVert->setMinimum( 0 );
	m_metaDirectionVert->setMaximum( 4 );
	connect( m_metaDirectionVert, SIGNAL( valueChanged( int ) ), this, SLOT( onMetaChangeDial( int ) ) ); // TODO: horizontal/vertical override each other

	m_metaLevel = new BetterSlider( this );
	m_metaLevel->setMinimum( 0 );
	m_metaLevel->setMaximum( UINT16_MAX ); // lol
	m_metaLevel->setOrientation( Qt::Horizontal );
	m_metaLevel->setTickPosition( QSlider::TicksBothSides );
	connect( m_metaLevel, SIGNAL( valueChanged( int ) ), this, SLOT( onMetaChangeSlider( int ) ) );

	// TODO: connect bitfield to meta
	m_metaBitField		 = new QWidget( this );
	m_metaBitFieldLayout = new QGridLayout( this );
	m_metaBitField->setLayout( m_metaBitFieldLayout );
	for ( int i = 0; i < 16; i++ )
	{
		QCheckBox *check = new QCheckBox( m_metaBitField );

		m_metaBitFieldLayout->addWidget( check, i / 4, i % 4 );

		m_metaBitFieldList.push_back( check );
	}

	m_metaToggle = new QCheckBox( this );
	connect( m_metaToggle, SIGNAL( toggled( bool ) ), this, SLOT( onMetaToggle( bool ) ) );

	m_metaRaw->setVisible( false );
	m_metaColor->setVisible( false );
	m_metaDirectionHorz->setVisible( false );
	m_metaDirectionVert->setVisible( false );
	m_metaLevel->setVisible( false );
	m_metaToggle->setVisible( false );
	m_metaOtherBlock->setVisible( false );
	m_metaBitField->setVisible( false );

	showAppropriateMetaThingie();
}

uint16_t BlockPropertyDialog::getChosenId() { return id; }
uint16_t BlockPropertyDialog::getChosenMeta() { return id; }

void BlockPropertyDialog::onIdSelectionChange( int index )
{
	id = index;
	showAppropriateMetaThingie();
}

void BlockPropertyDialog::showAppropriateMetaThingie()
{
	if ( m_layout->rowCount() > 1 )
	{
		auto w = m_layout->takeRow( 1 );
		w.fieldItem->widget()->setVisible( false );
		w.labelItem->widget()->setVisible( false );
	}

	m_metaLabel->setVisible( true );

	switch ( defs->value( id ).metaType )
	{
		case META_NONE:
			// Display Nothing
			m_metaLabel->setVisible( false );
			break;
		case META_COLOR:
			m_metaLabel->setText( tr( "Color:" ) );
			m_layout->addRow( m_metaLabel, m_metaColor );
			m_metaColor->setVisible( true );

			m_metaColor->setColorFromVTY( meta );

			break;
		case META_DIRECTION_HORIZ:
			m_metaLabel->setText( tr( "Rotation:" ) );
			m_layout->addRow( m_metaLabel, m_metaDirectionHorz );
			m_metaDirectionHorz->setVisible( true );

			m_metaDirectionHorz->setValue( meta );

			break;
		case META_DIRECTION_VERT:
			m_metaLabel->setText( tr( "Vertical Rotation:" ) );
			m_layout->addRow( m_metaLabel, m_metaDirectionVert );
			m_metaDirectionVert->setVisible( true );

			m_metaDirectionVert->setValue( meta );
			break;
		case META_DIRECTION:
			m_layout->addRow( m_metaDirectionHorz, m_metaDirectionVert );
			m_metaDirectionHorz->setVisible( true );
			m_metaDirectionVert->setVisible( true );
			m_metaLabel->setVisible( false );

			// TODO: this properly
			m_metaDirectionHorz->setValue( meta );
			m_metaDirectionVert->setValue( meta );
			break;
		case META_LEVEL:
			m_metaLabel->setText( tr( "Level:" ) );
			m_layout->addRow( m_metaLabel, m_metaLevel );
			m_metaLevel->setVisible( true );
			m_metaLevel->setMinimum( defs->value( id ).metaMin );
			m_metaLevel->setMaximum( defs->value( id ).metaMax );

			m_metaLevel->setValue( meta );
			break;
		case META_TOGGLE:
			m_metaLabel->setText( tr( "On:" ) );
			m_layout->addRow( m_metaLabel, m_metaToggle );
			m_metaToggle->setVisible( true );

			m_metaToggle->setChecked( meta );
			break;
		case META_BITFIELD:
			m_metaLabel->setText( tr( "Flags:" ) );
			m_layout->addRow( m_metaLabel, m_metaBitField );
			m_metaBitField->setVisible( true );

			for ( int i = 0; i < 16; i++ )
			{
				m_metaBitFieldList[i]->setChecked( meta & ( 1 << i ) );
			}
			break;
		case META_OTHERBLOCK:
			m_metaLabel->setText( tr( "Secondary ID:" ) );
			m_layout->addRow( m_metaLabel, m_metaOtherBlock );
			m_metaOtherBlock->setVisible( true );

			m_metaOtherBlock->setCurrentIndex( meta );
			break;
		default:
			m_metaLabel->setText( "Meta (raw):" );
			m_metaRaw->setText( QString::number( meta ) );
			m_layout->addRow( m_metaLabel, m_metaRaw );
			m_metaRaw->setVisible( true );

			m_metaRaw->setText( QString::number( meta ) );
			break;
	}
}


// The different slots for when the meta value changes
void BlockPropertyDialog::onMetaChangeLineEdit( const QString &text )
{
	meta = text.toInt();
}

void BlockPropertyDialog::onMetaChangeSlider( int value )
{
	meta = value;
}

void BlockPropertyDialog::onMetaChangeDial( int value )
{
	meta = value;
}

void BlockPropertyDialog::onMetaChangeCheckBox( bool checked )
{
	meta = checked;
}

void BlockPropertyDialog::onMetaChangeComboBox( int index )
{
	meta = index;
}

void BlockPropertyDialog::onMetaChangeColorPicker( const QColor &color )
{
	qDebug() << "Color changed to" << color;
	meta = m_metaColor->getVTYColor();

	qDebug() << "Meta is now" << meta;
}