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
#include <QListWidget>

#include "editor/entity.hpp"

EntityPropertyDialog::EntityPropertyDialog( EditorState *editorState, Entity *ent,  QWidget *parent ) : QDialog( parent )
{
    setWindowTitle( tr("Properties") );
    setMinimumSize( 640, 480 );

    m_editorState = editorState;
    m_entity = ent;
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
        m_nameEdit->setText( m_entity->m_entityType );
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
        m_properties->setSortingEnabled( false );

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

                connect( m_stringPropertyEdit, SIGNAL( editingFinished( ) ), this, SLOT( onStringPropertyEditFinished( ) ) );

                m_listPropertyEdit = new QComboBox( this );
                propertyEditGroupLayout->addWidget( m_listPropertyEdit );
                m_listPropertyEdit->setVisible( false );

                connect( m_listPropertyEdit, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onListPropertyEditChanged( int ) ) );
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
    
        resetProperties();

        // Make sure atleast the first row is selected
        if ( m_properties->rowCount() > 0 )
        {
            m_properties->selectRow( 0 );
        }
    }
    // End general tab

    QWidget *flagsTab = new QWidget( this );
    tabWidget->addTab( flagsTab, tr("Flags") );

    // Flags tab (TODO)
    {
        QVBoxLayout *tabLayout = new QVBoxLayout( flagsTab );
        flagsTab->setLayout( tabLayout );

        QListWidget *flagsList = new QListWidget( this );
        tabLayout->addWidget( flagsList );

        // int flagVal = 0;
        // for ( QString flag : m_entityDef.flags )
        // {
        //     QListWidgetItem *item = new QListWidgetItem( flagsList );
        //     item->setText( flag );
        //     item->setData( Qt::UserRole, 1 << flagVal );
        //     item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
        //     item->setCheckState( Qt::Unchecked );

        //     flagVal++;
        // }
    }
    // End flags tab

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
void EntityPropertyDialog::onSmartEditToggled( bool checked )
{
    m_smartEdit = checked;

    // HACK: force a refresh of the properties
    emit resetProperties( );
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
        QString type = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).type;

        if ( type == "STRING" )
        {
            m_stringPropertyEdit->setVisible( true );
            m_stringPropertyEdit->setText( valueItem->text() );
        }
        else if ( type == "LIST" )
        {
            m_listPropertyEdit->setVisible( true );
            m_listPropertyEdit->clear();

            // The list of values is stored as a "id:value,id:value,id:value,..." string
            QStringList values = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).min.split( "," );

            for ( QString value : values )
            {
                // Add the value to the list
                QString valueName = value.split( ":" ).last();
                m_listPropertyEdit->addItem( valueName );
            }

            // if current value is a number, set it as the index
            if ( m_entity->m_properties.contains( key ) )
            {
                int index = m_listPropertyEdit->findText( m_entity->m_properties[ key ] );
                if ( index != -1 )
                {
                    m_listPropertyEdit->setCurrentIndex( index );
                }
                // Otherwise, set to default
                else
                {
                    // Otherwise, set it to the default value
                    int defaultIndex = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).max.toInt();
                    m_listPropertyEdit->setCurrentIndex( defaultIndex );
                }
            }
            else
            {
                // Otherwise, set it to the default value
                int defaultIndex = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).max.toInt();
                m_listPropertyEdit->setCurrentIndex( defaultIndex );
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
        m_valueEdit->setText( m_entity->m_properties[ key ] );
    }
}

// Contrary to the name, it does not reset all to defaults
// Instead, it deletes the properties list and remakes it
void EntityPropertyDialog::resetProperties()
{
    // disable signals
    m_properties->blockSignals( true );

    int selectedRow = m_properties->currentRow();

    m_properties->clearContents();
    m_properties->setRowCount( 0 );

    // Add the properties
    for ( QString key : m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.keys() )
    {
        QString nameToDisplay = key;
        QString valueToDisplay = m_entity->m_properties.value( key, m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).min );

        if (m_smartEdit)
        {
            nameToDisplay = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).friendlyName;

            // If it's a list, display the label instead of the value
            if ( m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).type == "LIST" )
            {
                QStringList values = m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).min.split( "," );
                valueToDisplay = values.value( valueToDisplay.toInt() ).split(":").last();
            }
        }

        QTableWidgetItem *name = new QTableWidgetItem( nameToDisplay );
        QTableWidgetItem *value = new QTableWidgetItem( valueToDisplay );
        
        name->setToolTip( m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).description );
        value->setToolTip( m_editorState->m_pEntityDefs->value( m_entity->m_entityType ).properties.value( key ).description );

        // Store the actual key in the item data
        name->setData( Qt::UserRole, key );
        value->setData( Qt::UserRole, key );

        m_properties->insertRow( m_properties->rowCount() );
        m_properties->setItem( m_properties->rowCount() - 1, 0, name );
        m_properties->setItem( m_properties->rowCount() - 1, 1, value );
    }

    // select the row that was selected before (or try to)
    m_properties->selectRow( qMin( selectedRow, m_properties->rowCount() - 1 ) );

    // enable signals
    m_properties->blockSignals( false );
}

void EntityPropertyDialog::onStringPropertyEditFinished()
{
    QTableWidgetItem *nameItem = m_properties->selectedItems().first();
    QTableWidgetItem *valueItem = m_properties->selectedItems().last();
    if (!nameItem || !valueItem)
        return;

    QString key = nameItem->data( Qt::UserRole ).toString();

    QString value = m_stringPropertyEdit->text();
    m_entity->m_properties[ key ] = value;

    // HACK: force a refresh of the properties
    emit resetProperties();
}

void EntityPropertyDialog::onListPropertyEditChanged( int index )
{
    QTableWidgetItem *nameItem = m_properties->selectedItems().first();
    QTableWidgetItem *valueItem = m_properties->selectedItems().last();
    if (!nameItem || !valueItem)
        return;

    QString key = nameItem->data( Qt::UserRole ).toString();
    m_entity->m_properties[ key ] = QString::number( index );

    // HACK: force a refresh of the properties
    emit resetProperties();
}