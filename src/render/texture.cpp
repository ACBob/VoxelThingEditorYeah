#include "texture.hpp"

namespace render
{
    Vector4f getUV( const BlockDef& def ) {
        // texture coordinates
        // TODO: texture is assumed to be a 16x16 grid
        // so one indice is a sixteenth of the texture

        Vector4f uv;
        uv.x = (float)def.texturePos.x() / 16.0f;
        uv.y = (float)def.texturePos.y() / 16.0f;
        uv.x = uv.x + 1.0f / 16.0f;
        uv.y = uv.y + 1.0f / 16.0f;

        return uv;
    }

    Vector4f getUV(BlockDefs *defs, uint16_t type) {
        return getUV(defs->value(type));
    }
} // namespace render
