//
// Created by realxie on 2019-10-14.
//

#ifndef ALITA_VKCOMMANDENCODER_H
#define ALITA_VKCOMMANDENCODER_H

#include "VKDevice.h"

#include "../../include/Macros.h"
#include "../../include/CommandEncoder.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"



#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKCommandBuffer;
class VKRenderPassEncoder;

class VKCommandEncoder : public CommandEncoder
{
public:
    VKCommandEncoder(VKDevice* device);
    ~VKCommandEncoder();

public:
    virtual RenderPassEncoder* BeginRenderPass(/*GPURenderPassDescriptor descriptor*/) override;

    virtual CommandBuffer* Finish() override ;

private:
    VKDevice*               device_             = nullptr;
    VKCommandBuffer*        commandBuffer_      = nullptr;
    VKRenderPassEncoder*    renderPassEncoder_  = nullptr;
};

NS_RHI_END


#endif //ALITA_VKCOMMANDENCODER_H
