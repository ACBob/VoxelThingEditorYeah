#include "settingsdialog.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>

#include <QSettings>

#include "ui/widgets/colorpicker.hpp"

#include "editor/editorstate.hpp"

SettingsDialog::SettingsDialog( EditorState *editorState, QWidget *parent ) : QDialog( parent )
{
	QSettings settings;
	setWindowTitle( tr( "Settings" ) );

	m_editorState = editorState;

	QVBoxLayout *mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );

	QTabWidget *tabWidget = new QTabWidget( this );
	mainLayout->addWidget( tabWidget );

	QHBoxLayout *buttonLayout = new QHBoxLayout( this );
	mainLayout->addLayout( buttonLayout );

	QPushButton *okButton = new QPushButton( tr( "OK" ), this );
	buttonLayout->addWidget( okButton );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

	QPushButton *cancelButton = new QPushButton( tr( "Cancel" ), this );
	buttonLayout->addWidget( cancelButton );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

	// Tabs
	QWidget *tabGeneral = new QWidget( this );
	tabWidget->addTab( tabGeneral, tr( "General" ) );

	QWidget *tabGame = new QWidget( this );
	tabWidget->addTab( tabGame, tr( "Game" ) );

	// General
	QFormLayout *generalLayout = new QFormLayout( tabGeneral );
	tabGeneral->setLayout( generalLayout );

	QLabel *label = new QLabel( tr( "Grid Color:" ), tabGeneral );

	m_gridColorPicker = new ColorPicker( tabGeneral );
	m_gridColorPicker->enableAlpha();
	m_gridColorPicker->setColor( settings.value( "gridColor", QColor( Qt::gray ) ).value<QColor>() );

	generalLayout->addRow( label, m_gridColorPicker );

	QLabel *label2 = new QLabel( tr( "Void Color:" ), tabGeneral );

	m_voidColorPicker = new ColorPicker( tabGeneral );
	m_voidColorPicker->setColor( settings.value( "voidColor", QColor( Qt::black ) ).value<QColor>() );

	generalLayout->addRow( label2, m_voidColorPicker );

	// Game
	QFormLayout *gameLayout = new QFormLayout( tabGame );
	tabGame->setLayout( gameLayout );

	QLabel *label3 = new QLabel( tr( "Game Definition:" ), tabGame );

	m_gameDefComboBox = new QComboBox( tabGame );
	for ( GameDef &gameDef : *m_editorState->gameDefs )
	{
		m_gameDefComboBox->addItem( gameDef.id );
	}

	gameLayout->addRow( label3, m_gameDefComboBox );
}

void SettingsDialog::accept()
{
	QSettings settings;
	settings.setValue( "gridColor", m_gridColorPicker->getColor() );
	settings.setValue( "voidColor", m_voidColorPicker->getColor() );
	settings.setValue( "gameDef", m_gameDefComboBox->currentText() );
	QDialog::accept();
}