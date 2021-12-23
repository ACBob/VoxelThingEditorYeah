#include "entity.hpp"

#include "editorstate.hpp"

Entity::Entity( EditorState *editorState, QString type, QObject *parent ) : QObject( parent )
{
    m_editorState = editorState;
    m_entityType = type;

    if (!m_editorState->m_pEntityDefs->contains(m_entityType))
    {
        qDebug() << "Entity type" << m_entityType << "not found";
        return;
    }

    m_properties = QMap<QString, QString>();
    for ( auto &k : m_editorState->m_pEntityDefs->value( type ).properties )
    {
        m_properties.insert( k.name, k.defaultValue );
    }
}