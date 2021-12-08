// -- Texture!
// Handles some texture stuff w/ Qt, plus lets you get a texture from an ID with a block definition.

#include "editor/blockdefs.hpp"

#include "world/vector.hpp"

namespace render {
    Vector4f getUV(const BlockDef& def);
    Vector4f getUV(BlockDefs *defs, uint16_t id);
}