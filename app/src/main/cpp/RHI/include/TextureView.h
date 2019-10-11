//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_TEXTUREVIEW_H
#define ALITA_TEXTUREVIEW_H

#include "Macros.h"

#include <memory>

NS_RHI_BEGIN

class TextureView;

typedef std::shared_ptr<TextureView> TextureViewHnd;

class TextureView
{
public:
    virtual ~TextureView()
    {
    }
};

NS_RHI_END

#endif //ALITA_TEXTUREVIEW_H
