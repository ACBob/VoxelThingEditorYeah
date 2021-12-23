#include "properties.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QLineEdit>
#include <QDial>
#include <QComboBox>

EntityPropertyDialog::EntityPropertyDialog( EditorState *editorState, EntityDef begin,  QWidget *parent ) : QDialog( parent )
{
    setWindowTitle( tr("Properties") );
    setMinimumSize( 640, 480 );

    m_editorState = editorState;
    m_entityDef = begin;
    m_smartEdit = true;


    QVBoxLayout *layout = new QVBoxLayout( this );
    setLayout( layout );

    QHBoxLayout *headerLayout = new QHBoxLayout( this );
    layout->addLayout( headerLayout );

    // Header
    {
        QVBoxLayout *leftLayout = new QVBoxLayout( this );
        headerLayout->addLayout( leftLayout );

        m_nameEdit = new QLineEdit( this );
        m_nameEdit->setText( m_entityDef.entityType );
        leftLayout->addWidget( m_nameEdit );

        QHBoxLayout *leftButtonsLayout = new QHBoxLayout( this );
        leftLayout->addLayout( leftButtonsLayout );

        QPushButton *copyButton = new QPushButton( tr("Copy"), this );
        leftButtonsLayout->addWidget( copyButton );

        QPushButton *pasteButton = new QPushButton( tr("Paste"), this );
        leftButtonsLayout->addWidget( pasteButton );

        headerLayout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding ) );

        QGridLayout *rightLayout = new QGridLayout( this );
        headerLayout->addLayout( rightLayout );
        
        QPushButton *smartEditToggle = new QPushButton( tr("Smart Edit"), this );
        smartEditToggle->setCheckable( true );
        smartEditToggle->setChecked( m_smartEdit );
        rightLayout->addWidget( smartEditToggle, 0, 0 );

        QPushButton *helpButton = new QPushButton( tr("Help"), this );
        rightLayout->addWidget( helpButton, 1, 0 );

        connect( smartEditToggle, SIGNAL( toggled( bool ) ), this, SLOT( onSmartEditToggled( bool ) ) );

        m_rotationDial = new QDial( this );
        m_rotationDial->setRange( 0, 359 );
        m_rotationDial->setWrapping( true );
        m_rotationDial->setNotchesVisible( true );
        m_rotationDial->setNotchTarget( 45 );
        rightLayout->addWidget( m_rotationDial, 0, 1 );

        QLineEdit *rawRotationInput = new QLineEdit( this );
        rawRotationInput->setText( "0" );
        rightLayout->addWidget( rawRotationInput, 1, 1 );
    }
    // End header


    QTabWidget *tabWidget = new QTabWidget( this );
    layout->addWidget( tabWidget );

    QWidget *generalTab = new QWidget( this );
    tabWidget->addTab( generalTab, tr("General") );

    // General tab
    {
        QHBoxLayout *tabLayout = new QHBoxLayout( generalTab );
        generalTab->setLayout( tabLayout );

        m_properties = new QTableWidget( this );
        tabLayout->addWidget( m_properties );
        m_properties->setColumnCount( 2 );
        m_properties->setHorizontalHeaderLabels( QStringList() << tr("Name") << tr("Value") );
        m_properties->setSelectionMode( QAbstractItemView::SingleSelection );
        m_properties->setSelectionBehavior( QAbstractItemView::SelectRows );
        m_properties->setEditTriggers( QAbstractItemView::NoEditTriggers );
        m_properties->setAlternatingRowColors( true );
        m_properties->setSortingEnabled( true );

        m_properties->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
        m_properties->verticalHeader()->setVisible( false );

        connect( m_properties, SIGNAL( itemSelectionChanged( ) ), this, SLOT( onSelectionChanged( ) ) );

        QWidget *propertyEditHolder = new QWidget( this );
        tabLayout->addWidget( propertyEditHolder );

        QVBoxLayout *propertyEditLayout = new QVBoxLayout( propertyEditHolder );
        propertyEditHolder->setLayout( propertyEditLayout );

        // Property Edit
        {
            QGroupBox *propertyEditGroup = new QGroupBox( this );
            propertyEditLayout->addWidget( propertyEditGroup );
            
            QVBoxLayout *propertyEditGroupLayout = new QVBoxLayout( propertyEditGroup );
            propertyEditGroup->setLayout( propertyEditGroupLayout );

            // The various "smart" editable types
            {
                m_stringPropertyEdit = new QLineEdit( this );
                propertyEditGroupLayout->addWidget( m_stringPropertyEdit );
                m_stringPropertyEdit->setVisible( false );

                m_listPropertyEdit = new QComboBox( this );
                propertyEditGroupLayout->addWidget( m_listPropertyEdit );
                m_listPropertyEdit->setVisible( false );
            }
            // End smart editable types

            // Outside of Smart Edit
            {
                m_keyEdit = new QLineEdit( this );
                propertyEditGroupLayout->addWidget( m_keyEdit );

                m_valueEdit = new QLineEdit( this );
                propertyEditGroupLayout->addWidget( m_valueEdit );

                QHBoxLayout *propertyEditButtons = new QHBoxLayout( this );
                propertyEditGroupLayout->addLayout( propertyEditButtons );

                m_addKeyButton = new QPushButton( tr("Add"), this );
                propertyEditButtons->addWidget( m_addKeyButton );

                m_removeKeyButton = new QPushButton( tr("Remove"), this );
                propertyEditButtons->addWidget( m_removeKeyButton );

                m_addKeyButton->setVisible( false );
                m_removeKeyButton->setVisible( false );
                m_keyEdit->setVisible( false );
                m_valueEdit->setVisible( false );
            }
            // End outside of Smart Edit

            // Top-align
            propertyEditGroupLayout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
        }
        // End property edit
    
        for ( QString key : m_entityDef.properties.keys() )
        {
            QTableWidgetItem *name = new QTableWidgetItem( m_entityDef.properties[key].name );
            QTableWidgetItem *value = new QTableWidgetItem( m_entityDef.properties[key].value );

            name->setToolTip( m_entityDef.properties[key].description );
            value->setToolTip( m_entityDef.properties[key].description );

            // Store the actual key in the item data
            name->setData( Qt::UserRole, key );
            value->setData( Qt::UserRole, key );

            m_properties->insertRow( m_properties->rowCount() );
            m_properties->setItem( m_properties->rowCount() - 1, 0, name );
            m_properties->setItem( m_properties->rowCount() - 1, 1, value );
        }

        // Make sure atleast the first row is selected
        if ( m_properties->rowCount() > 0 )
        {
            m_properties->selectRow( 0 );
        }
    }
    // End general tab

    QWidget *flagsTab = new QWidget( this );
    tabWidget->addTab( flagsTab, tr("Flags") );

    // TODO: flags tab

    QHBoxLayout *footerLayout = new QHBoxLayout( this );
    layout->addLayout( footerLayout );
    footerLayout->addSpacerItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding ) );

    QPushButton *okButton = new QPushButton( tr("Ok"), this );
    footerLayout->addWidget( okButton );

    QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );
    footerLayout->addWidget( cancelButton );

    connect( okButton, SIGNAL( clicked( ) ), this, SLOT( accept( ) ) );
    connect( cancelButton, SIGNAL( clicked( ) ), this, SLOT( reject( ) ) );
}

void EntityPropertyDialog::setEntityDef( const EntityDef &def )
{
    m_entityDef = def;
}

const EntityDef &EntityPropertyDialog::getEntityDef()
{
    return m_entityDef;
}

void EntityPropertyDialog::onSmartEditToggled( bool checked )
{
    m_smartEdit = checked;

    // HACK: force a refresh of the properties
    emit onSelectionChanged( );
}

void EntityPropertyDialog::onNameChanged( const QString &name )
{
    // TODO:
    // m_entityDef.name = name;
}

void EntityPropertyDialog::onRotationChanged( int value )
{
    // TODO:
    // m_entityDef.rotation = value;
}

void EntityPropertyDialog::onSelectionChanged( )
{
    QTableWidgetItem *nameItem = m_properties->selectedItems().first();
    QTableWidgetItem *valueItem = m_properties->selectedItems().last();
    if (!nameItem || !valueItem)
        return;

    QString key = nameItem->data( Qt::UserRole ).toString();

    m_addKeyButton->setVisible( false );
    m_removeKeyButton->setVisible( false );
    m_keyEdit->setVisible( false );
    m_valueEdit->setVisible( false );
    m_stringPropertyEdit->setVisible( false );
    m_listPropertyEdit->setVisible( false );

    if ( m_smartEdit )
    {
        QString type = m_entityDef.properties[key].type;

        if ( type == "STRING" )
        {
            m_stringPropertyEdit->setVisible( true );
            m_stringPropertyEdit->setText( valueItem->text() );
        }
        else if ( type == "LIST" )
        {
            m_listPropertyEdit->setVisible( true );
            m_listPropertyEdit->clear();

            // The list of values is stored as "defaultIndex,value1,value2,value3" in the default value
            // And then the value is set to an index of the list
            QStringList values = m_entityDef.properties[key].defaultValue.split( "," );
            // remove the first
            values.removeFirst();

            for ( QString value : values )
            {
                // Add the value to the list
                m_listPropertyEdit->addItem( value );    
            }

            // if current value is a number, set it as the index
            if ( valueItem->text().toInt() >= 0 )
            {
                m_listPropertyEdit->setCurrentIndex( valueItem->text().toInt() );
            }
            else
            {
                // Otherwise, set it to the first item
                m_listPropertyEdit->setCurrentIndex( 0 );
            }
        }
    }
    else
    {
        m_addKeyButton->setVisible( true );
        m_removeKeyButton->setVisible( true );
        m_keyEdit->setVisible( true );
        m_valueEdit->setVisible( true );

        m_keyEdit->setText( key );
        m_valueEdit->setText( valueItem->text() );
    }

    update();
}