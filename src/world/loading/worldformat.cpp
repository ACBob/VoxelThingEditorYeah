#include "worldformat.hpp"

#include "../world.hpp"
#include "../chunk.hpp"

#include <QFile>

#include <QDebug>

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


bool 