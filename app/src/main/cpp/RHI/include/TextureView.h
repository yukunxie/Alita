//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_TEXTUREVIEW_H
#define ALITA_TEXTUREVIEW_H

#include "Macros.h"
#include "RHIObjectBase.h"



NS_RHI_BEGIN

class TextureView;

class TextureView : public RHIObjectBase
{
public:
    virtual ~TextureView()
    {
    }

    virtual TextureFormat  GetFormat() const = 0;
};

NS_RHI_END

#endif //ALITA_TEXTUREVIEW_H
