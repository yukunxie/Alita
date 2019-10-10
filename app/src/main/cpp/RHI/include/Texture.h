//
// Created by realxie on 2019-10-07.
//

#ifndef ALITA_TEXTURE_H
#define ALITA_TEXTURE_H

#include "Macros.h"

#include <memory>

NS_RHI_BEGIN

class Texture;

typedef std::shared_ptr<Texture> TextureHnd;

class Texture
{
public:
    virtual ~Texture()
    {
    }
};

NS_RHI_END

#endif //ALITA_TEXTURE_H
