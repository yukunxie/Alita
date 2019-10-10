//
// Created by realxie on 2019-10-06.
//

#include "VKUniformBufferObject.h"
#include "VKTexture.h"
#include "VKSampler.h"
#include "VKBuffer.h"
#include "drivers/vulkan/vulkan_wrapper.h"

NS_RHI_BEGIN

VKUniformBufferObject::VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline)
{
    vkDevice_ = device->GetDevice();
    vkDescriptorSet_ = graphicPipeline->GetDescriptorSet();
}

VKUniformBufferObject::VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size):
        VKUniformBufferObject(device, graphicPipeline)
{
    buffer_ = (VKBuffer*) buffer;
    bindingPoint_ = bindingPoint;
    offset_ = offset;
    size_   = size;
}

VKUniformBufferObject::VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Texture* texture, const Sampler* sampler):
        VKUniformBufferObject(device, graphicPipeline)
{
    bindingPoint_ = bindingPoint;
    texture_ = (VKTexture*)texture;
    sampler_ = (VKSampler*)sampler;
}

void VKUniformBufferObject::UseUniformBufferObject()
{
    if (buffer_)
    {
        VkDescriptorBufferInfo bufferInfo{
                .buffer = buffer_->GetNative(),
                .offset = offset_,
                .range  = size_,
        };

        VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = vkDescriptorSet_,
                .dstBinding = bindingPoint_,
                .dstArrayElement = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .pBufferInfo = &bufferInfo,
                .pImageInfo = nullptr, // optional
                .pTexelBufferView = nullptr, // optional

        };
        vkUpdateDescriptorSets(vkDevice_, 1, &descriptorWrite, 0, nullptr);
    }
    else if (texture_ && sampler_)
    {
        VkDescriptorImageInfo imageInfo = {
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .imageView = texture_->GetNative(),
                .sampler = sampler_->GetNative(),
        };

        VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = vkDescriptorSet_,
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 1,
                .pBufferInfo = nullptr,
                .pImageInfo = &imageInfo, // optional
                .pTexelBufferView = nullptr, // optional

        };
        vkUpdateDescriptorSets(vkDevice_, 1, &descriptorWrite, 0, nullptr);
    }
    else
    {
        RHI_ASSERT(false);
    }
}

VKUniformBufferObject::~VKUniformBufferObject()
{
    buffer_  = nullptr;
    sampler_ = nullptr;
    texture_ = nullptr;
}

void VKUniformBufferObject::Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const
{
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &vkDescriptorSet_, 0, nullptr);

//    if (buffer_)
//    {
//        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, bindingPoint_, 1, &vkDescriptorSet_, 0, nullptr);
//    }
//    else if (texture_)
//    {
//
//    }

}

NS_RHI_END
