// -- Editor State
// Holds the state of the editor, something needed by just about everything.

#include <QString>

#pragma once

class CWorld;
class CTool;

// TODO: any way to forward-declare a using statement?
#include "blockdefs.hpp"

struct EditorState
{
    // The current filename.
    QString filename;

    // Pointer to the world object.
    CWorld *world;

    // Pointer to the current tool.
    CTool *tool;

    // Pointer to a QVector of block definitions.
    BlockDefs *blockDefs;

    // TODO: Pointer to a QVector of Entity definitions.
    // EntityDefs *entityDefs;
};