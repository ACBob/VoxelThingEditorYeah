// -- Editor State
// Holds the state of the editor, something needed by just about everything.

#include <QString>
#include <QObject>

#pragma once

class CWorld;
class CTool;

// TODO: any way to forward-declare a using statement?
#include "blockdefs.hpp"

class EditorState : public QObject
{
    Q_OBJECT

    public:
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

    // Holds the block type / meta we want to place.
    // Interpreted by tools differently.
    uint16_t chosenBlockType;
    uint16_t chosenBlockMeta;

    // The absolute path of the block texture.
    QString blockTexturePath;

    // Functions to set
    // These ideally should be what you call to set stuff, where-as you just access the variables directly.
    // These functions notifies the editor that something has changed, and the editor should update.
    void setFilename(QString filename) {
        this->filename = filename;
        emit filenameChanged(filename);
    }
    void setWorld(CWorld *world) {
        this->world = world;
        emit worldChanged(world);
    }
    void setTool(CTool *tool) {
        this->tool = tool;
        emit toolChanged(tool);
    }
    void setBlockDefs(BlockDefs *blockDefs) {
        this->blockDefs = blockDefs;
        emit blockDefsChanged(blockDefs);
    }
    void setChosenBlockType(uint16_t chosenBlockType) {
        this->chosenBlockType = chosenBlockType;
        emit chosenBlockTypeChanged(chosenBlockType);
    }
    void setChosenBlockMeta(uint16_t chosenBlockMeta) {
        this->chosenBlockMeta = chosenBlockMeta;
        emit chosenBlockMetaChanged(chosenBlockMeta);
    }
    void setBlockTexturePath(QString blockTexturePath) {
        this->blockTexturePath = blockTexturePath;
        emit blockTexturePathChanged(blockTexturePath);
    }

    signals:
    void filenameChanged(QString filename);
    void worldChanged(CWorld *world);
    void toolChanged(CTool *tool);
    void blockDefsChanged(BlockDefs *blockDefs);
    void chosenBlockTypeChanged(uint16_t chosenBlockType);
    void chosenBlockMetaChanged(uint16_t chosenBlockMeta);
    void blockTexturePathChanged(QString blockTexturePath);
};