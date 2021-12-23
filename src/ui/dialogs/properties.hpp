// -- Properties
// Generic property dialog for Entities
// Also used when editing the world properties

#include <QDialog>

#include "editor/editorstate.hpp"
#include "editor/defs.hpp"

class QDial;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QComboBox;

class EntityPropertyDialog : public QDialog
{
    Q_OBJECT;

    public:
        EntityPropertyDialog( EditorState *editorState, EntityDef begin,  QWidget *parent = 0 );

        void setEntityDef( const EntityDef &def );
        const EntityDef &getEntityDef();

    private slots:
        void onNameChanged( const QString &name );
        void onSmartEditToggled( bool checked );
        void onRotationChanged( int value );

        void onSelectionChanged( );

    private:
        EditorState *m_editorState;
        EntityDef m_entityDef;

        QDial *m_rotationDial;
        QLineEdit *m_nameEdit;
        QTableWidget *m_properties;

        // Outside of smart edit mode
        QPushButton *m_addKeyButton;
        QPushButton *m_removeKeyButton;
        QLineEdit *m_keyEdit;
        QLineEdit *m_valueEdit;

        // Different edit widgets for different types of properties
        QLineEdit *m_stringPropertyEdit;
        QComboBox *m_listPropertyEdit;

        bool m_smartEdit;
};