//
// Created by realxie on 2019-10-29.
//

#ifndef ALITA_VERTEXBUFFER_H
#define ALITA_VERTEXBUFFER_H

#include "../Base/ObjectBase.h"

#include <cstdint>

NS_RX_BEGIN

enum class VertexBufferKind
{
    POSITION,
    TEXCOORD,
    DIFFUSE,
    NORMAL,
    TANGENT,
    BINORMAL,
    BITANGENT,
    
    // must be the last
    MAX_COUNT
};

constexpr std::uint32_t VertexBufferKindCount = (std::uint32_t)VertexBufferKind::MAX_COUNT;

class VertexBuffer
{
};


NS_RX_END

#endif //ALITA_VERTEXBUFFER_H
