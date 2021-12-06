#include "settingsdialog.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QSettings>

#include "colorpicker.hpp"

SettingsDialog::SettingsDialog( QWidget *parent ) : QDialog( parent )
{
	QSettings settings;
	setWindowTitle( tr( "Settings" ) );

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
}

void SettingsDialog::accept()
{
	QSettings settings;
	settings.setValue( "gridColor", m_gridColorPicker->getColor() );
	settings.setValue( "voidColor", m_voidColorPicker->getColor() );
	QDialog::accept();
}