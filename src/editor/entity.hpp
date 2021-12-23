// -- Entity
// Instead of being the purely config-based entity, this is an entity that appears in-world
// and has the applied properties.

#include "defs.hpp"
#include "world/vector.hpp"

#include <QObject>

class EditorState;

class Entity : public QObject
{
    Q_OBJECT;

    public:
        Entity( EditorState *editorState, QString type, QObject *parent = nullptr );

        QMap<QString, QString> m_properties;

        Vector3f m_position;
        Vector3f m_rotation;

        QString m_entityType;

    private:
        EditorState *m_editorState;
};