//
// Created by realxie on 2019-10-11.
//

#ifndef ALITA_COMMANDENCODER_H
#define ALITA_COMMANDENCODER_H

#include "Macros.h"
#include "RHIObjectBase.h"
#include "CommandBuffer.h"
#include "RenderPassEncoder.h"

NS_RHI_BEGIN

class CommandEncoder : public RHIObjectBase
{
public:
    virtual ~CommandEncoder()
    {
    }

public:
    virtual RenderPassEncoder* BeginRenderPass(/*GPURenderPassDescriptor descriptor*/) = 0;

    virtual CommandBuffer* Finish() = 0;
};

NS_RHI_END


#endif //ALITA_COMMANDENCODER_H
