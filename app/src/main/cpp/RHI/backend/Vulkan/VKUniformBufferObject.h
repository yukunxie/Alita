//
// Created by realxie on 2019-10-06.
//

#ifndef ALITA_VKUNIFORMBUFFEROBJECT_H
#define ALITA_VKUNIFORMBUFFEROBJECT_H

#include "VKDevice.h"
#include "VKGraphicPipeline.h"
#include "../../include/Macros.h"
#include "../../include/UniformBufferObject.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKUniformBufferObject : public UniformBufferObject
{
public:
    VKUniformBufferObject() = default;
    VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline);
    virtual ~VKUniformBufferObject();

    VkDescriptorSet GetNative() const {return vkDescriptorSet_;}
private:
    VkDescriptorSet  vkDescriptorSet_ = 0;
};

NS_RHI_END


#endif //ALITA_VKUNIFORMBUFFEROBJECT_H
