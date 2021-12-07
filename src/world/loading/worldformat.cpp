#include "worldformat.hpp"

#include "../world.hpp"
#include "../chunk.hpp"

#include <QFile>
#include <QDir>

#include <QDebug>

#include "editor/toml.hpp"

#define Pull32bit(var, dat) \
    var = (dat[0] << 24) | (dat[1] << 16) | (dat[2] << 8) | dat[3]; \
    dat += 4

#define Pull16bit(var, dat) \
    var = (dat[0] << 8) | dat[1]; \
    dat += 2

#define Pull8bit(var, dat) \
    var = dat[0]; \
    dat += 1

// I hate binary blob formats.
bool Meegreef1Format::Load(CWorld *world, QString filePath) {
    qFatal("Meegreef World format not implemented");
    return false;
}

bool Meegreef1Format::Save(CWorld *world, QString file) {
    return false;
}

// VoxelFormatYeah

using uint16 = quint16;
using uint32 = quint32;
using uint8 = quint8;
using int16 = qint16;
using int32 = qint32;
using int8 = qint8;

bool VoxelFormatYeah::Load(CWorld *world, QString filePath) {
    
    // Open the 'meta.toml' file.
    QDir d = QDir::cleanPath(filePath);

    if (!d.exists()) {
        qWarning() << "VoxelFormatYeah: Could not find meta.toml";
        return false;
    }

    QFile f(d.absoluteFilePath("meta.toml"));
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "VoxelFormatYeah: Could not open meta.toml";
        return false;
    }

    QByteArray data = f.readAll();

    // Parse the toml file.
    toml::table root = toml::parse(data.toStdString());
    toml::table *metaRoot = root["meta"].as_table();

    // World version.
    auto *v = metaRoot->get("version");
    if (!v || !v->is_integer()) {
        qWarning() << "VoxelFormatYeah: Could not find version";
        return false;
    }

    uint32 version = v->value_or<uint32>(0);

    if ( version < VTYG_OLDEST_SUPPORTED_VERSION || version > VTYG_CURRENT_VERSION ) {
        qWarning() << "VoxelFormatYeah: Unsupported version";
        return false;
    }

    // World name.
    auto *n = metaRoot->get("name");
    if (!n || !n->is<std::string>()) {
        qWarning() << "VoxelFormatYeah: Could not find name";
        return false;
    }

    QString name = QString::fromStdString(n->value_or<std::string>("VoxelWorld"));

    // Chunk sizes
    auto *cs = metaRoot->get("chunk_sizes");
    if (!cs || !cs->is_array()) {
        qWarning() << "VoxelFormatYeah: Could not find chunk_sizes";
        return false;
    }

    Vector3c chunkSizes;
    auto *csArray = cs->as_array();
    chunkSizes.x = csArray->get(0)->value_or(1);
    chunkSizes.y = csArray->get(1)->value_or(1);
    chunkSizes.z = csArray->get(2)->value_or(1);

    world->setChunkSize(chunkSizes.x, chunkSizes.y, chunkSizes.z);
    world->setName(name);

    // Load the game name
    auto *g = metaRoot->get("game");
    if (!g || !g->is_string()) {
        qWarning() << "VoxelFormatYeah: Could not find game. Defaulting to Meegreef";
    }

    QString game = QString::fromStdString(g->value_or<std::string>("Meegreef"));

    // Load the world seed
    auto *s = metaRoot->get("seed");
    if (!s || !s->is_integer()) {
        qWarning() << "VoxelFormatYeah: Could not find seed. Defaulting to 0";
    }

    uint32 seed = s->value_or<uint32>(0);
    world->setSeed(seed);

    // TODO: Load dimensions, author info, etc.
    // This is just a bare-bones get-it-working version.

    // Load the chunks.
    // Assuming a dimension called "overworld"
    // load the chunks from the "overworld" directory.
    QDir dimensionDir = d.absoluteFilePath("overworld");
    if (!dimensionDir.exists()) {
        qWarning() << "VoxelFormatYeah: Could not find overworld directory";
        return false;
    }

    QDir chunkDir = dimensionDir.absoluteFilePath("chunks");
    // TODO: entity files

    if (!chunkDir.exists()) {
        qWarning() << "VoxelFormatYeah: Could not find chunks directory";
        return false;
    }

    QStringList chunkFiles = chunkDir.entryList(QDir::Files);

    world->clearChunks();

    for ( QString chunkFile : chunkFiles ) {
        QFile f(chunkDir.absoluteFilePath(chunkFile));
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning() << "VoxelFormatYeah: Could not open chunk file" << chunkFile;
            continue;
        }

        // Find the position of the chunk from the filename.
        // chunk_x_y_z.dat
        QStringList parts = chunkFile.split('_');
        if (parts.size() != 4) {
            qWarning() << "VoxelFormatYeah: Could not parse chunk filename" << chunkFile;
            continue;
        }

        int x = parts[1].toInt();
        int y = parts[2].toInt();
        int z = parts[3].toInt();

        // Read chunk data.
        QByteArray data = f.readAll();

        // Read the chunk header.
        char header[4];
        if (data.size() < 4) {
            qWarning() << "VoxelFormatYeah: Chunk file too small" << chunkFile;
            continue;
        }

        memcpy(header, data.data(), 4);

        // Check the header.
        if ( header[0] != 'V' || header[1] != 'T' || header[2] != 'Y' || header[3] != 'G' ) {
            qWarning() << "VoxelFormatYeah: Invalid chunk header" << chunkFile;
            continue;
        }

        // Now test it against
        // EMPTY, UNGENERATED, FILLED, or a regular chunk.
        // TODO: Right now only have EMPTY and regular chunks.

        // EMPTY
        // Read the next 5 bytes for the word "EMPTY".
        if (data.size() < 5) {
            qWarning() << "VoxelFormatYeah: Chunk file too small" << chunkFile;
            continue;
        }

        char empty[5];
        memcpy(empty, data.data() + 4, 5);

        if ( empty[0] == 'E' && empty[1] == 'M' && empty[2] == 'P' && empty[3] == 'T' && empty[4] == 'Y' ) {
            // Create an empty chunk.
            CChunk *chunk = world->createChunk(x, y, z);
            continue;
        }

        // Otherwise, it's a regular chunk.
        CChunk *chunk = world->createChunk(x, y, z);

        if (!chunk) {
            qWarning() << "VoxelFormatYeah: Could not create chunk" << chunkFile;
            continue;
        }

        // chunk size is sizeX * sizeY * sizeZ
        // chunk data is sizeX * sizeY * sizeZ * 4 bytes, and then another sizeX * sizeY * sizeZ * 2 bytes for the lighting.
        if (data.size() < (chunkSizes.x * chunkSizes.y * chunkSizes.z * 4)) { // TODO: lighting + chunkSizes.x * chunkSizes.y * chunkSizes.z * 2)) {
            qWarning() << "VoxelFormatYeah: Chunk file too small" << chunkFile;
            continue;
        }

        // Read the chunk data.
        for ( int i = 0; i < chunkSizes.x * chunkSizes.y * chunkSizes.z; i++ ) {
            uint32 voxel = *(uint32 *)(data.data() + 4 + i * 4);
            chunk->set(i, voxel);
        }

        chunk->rebuildModel();

        // Read the chunk lighting.
        // for ( int i = 0; i < chunkSizes.x * chunkSizes.y * chunkSizes.z; i++ ) {
        //     uint32 lighting = *(uint32 *)(data.data() + 4 + chunkSizes.x * chunkSizes.y * chunkSizes.z * 4 + i * 4);
        //     chunk->setLighting(i, lighting);
        // }
    }

    return true;
}

bool VoxelFormatYeah::Save(CWorld *world, QString filename)
{

    // If the folder doesn't exist, create it.
    QDir d(filename);
    if (!d.exists()) {
        if (!d.mkpath(d.absolutePath())) {
            qWarning() << "VoxelFormatYeah: Could not create directory" << d.absolutePath();
            return false;
        }
    }

    // Create the root object.
    toml::table root = toml::table{{
        {"meta", toml::table{{
            {"name", world->getName().toStdString()},
            {"seed", world->getSeed()},
            {"game", "Meegreef"}, // TODO
            {"version", VTYG_CURRENT_VERSION},
            {"chunk_sizes", toml::array{world->getChunkSize().x, world->getChunkSize().y, world->getChunkSize().z}},
        }}},

        // dimensions.overworld
        {"dimensions.overworld", toml::table{{
            {"time", world->getTime()},
            {"name", "overworld"},
        }}},
    }};

    // write the file to meta.toml
    QFile meta(d.absoluteFilePath("meta.toml"));
    if (!meta.open(QIODevice::WriteOnly)) {
        qWarning() << "VoxelFormatYeah: Could not open meta.toml";
        return false;
    }

    // toml wants a ostream
    // so we write it to an ostream, then read back the buffer.
    std::ostringstream oss;
    oss << root;
    QByteArray data = oss.str().c_str();
    meta.write(data);
    qDebug() << "VoxelFormatYeah: meta.toml:" << data;

    // Create overworld directory.
    QDir dimensionDir = d.absoluteFilePath("overworld");
    if (!dimensionDir.exists()) {
        if (!dimensionDir.mkpath(dimensionDir.absolutePath())) {
            qWarning() << "VoxelFormatYeah: Could not create directory" << dimensionDir.absolutePath();
            return false;
        }
    }

    // Create chunks directory.
    QDir chunkDir = dimensionDir.absoluteFilePath("chunks");
    if (!chunkDir.exists()) {
        if (!chunkDir.mkpath(chunkDir.absolutePath())) {
            qWarning() << "VoxelFormatYeah: Could not create directory" << chunkDir.absolutePath();
            return false;
        }
    }

    // Now save the chunks.

    for (CChunk *chunk : world->getChunks()) {
        // Get the chunk position.
        int x = chunk->getX();
        int y = chunk->getY();
        int z = chunk->getZ();

        // Create the chunk file.
        QFile f(chunkDir.absoluteFilePath(QString("chunk_%1_%2_%3.dat").arg(x).arg(y).arg(z)));
        if (!f.open(QIODevice::WriteOnly)) {
            qWarning() << "VoxelFormatYeah: Could not open chunk file" << f.fileName();
            return false;
        }

        // Write the header.
        f.write("VTYG", 4);

        // if all blocks are equal, write EMPTY
        bool allEqual = true;
        uint32 voxel = chunk->get(0);
        for (int i = 1; i < chunk->getSize().x * chunk->getSize().y * chunk->getSize().z; i++) {
            if (chunk->get(i) != voxel) {
                allEqual = false;
                break;
            }
        }

        if (allEqual) {
            f.write("EMPTY", 5);
        } else {
            // Write the chunk data.
            for ( int i = 0; i < chunk->getSize().x * chunk->getSize().y * chunk->getSize().z; i++ ) {
                uint32 voxel = chunk->get(i);
                f.write((char *)&voxel, 4);
            }

            // Write the chunk lighting.
            // for ( int i = 0; i < chunk->getSize().x * chunk->getSize().y * chunk->getSize().z; i++ ) {
            //     uint32 lighting = chunk->getLighting(i);
            //     f.write((char *)&lighting, 4);
            // }
        }
    }
    return true;
}