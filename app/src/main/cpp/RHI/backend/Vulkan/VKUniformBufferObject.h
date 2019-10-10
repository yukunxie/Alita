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

class VKTexture;
class VKSampler;

class VKUniformBufferObject : public UniformBufferObject
{
public:
    VKUniformBufferObject() = delete;
    VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline);
    VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size);
    VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Texture* texture, const Sampler* sampler);
    virtual ~VKUniformBufferObject();

    VkDescriptorSet GetNative() const {return vkDescriptorSet_;}

    void UseUniformBufferObject();
    void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const;

private:
    VkDevice            vkDevice_           = nullptr;
    VkDescriptorSet     vkDescriptorSet_    = 0;
    const VKBuffer*     buffer_             = nullptr;
    const VKTexture*    texture_            = nullptr;
    const VKSampler*    sampler_            = nullptr;
    std::uint32_t       bindingPoint_       = 0;
    std::uint32_t       offset_             = 0;
    std::uint32_t       size_               = 0;
};

NS_RHI_END


#endif //ALITA_VKUNIFORMBUFFEROBJECT_H
