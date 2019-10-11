//
// Created by realxie on 2019-10-10.
//

#include "VKBindGroup.h"
#include "VKBuffer.h"
#include "VKTextureView.h"
#include "VKSampler.h"


NS_RHI_BEGIN

VKBindGroup::VKBindGroup(VKDevice* device, const VKBindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindResources)
{
    vkDevice_ = device->GetDevice();
    VkDescriptorSetLayout setLayout = bindGroupLayout->GetNative();
    VkDescriptorSetAllocateInfo allocInfo  {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorSetCount = 1,
            .pSetLayouts = &setLayout,
            .descriptorPool = device->GetDescriptorPool()
    };

    CALL_VK(vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, &vkDescriptorSet_));

    for (const auto res : bindResources)
    {
        bindingResources_.push_back(res);
    }
}

VKBindGroup::~VKBindGroup()
{
    // TODO release vulkan resource
}

void VKBindGroup::WriteToGPU() const
{
    for (const BindingResource* resource : bindingResources_)
    {
        if (resource->GetResourceType() == BindingResourceType::BUFFER)
        {
            auto* buffer = (const VKBindingBuffer*)resource;
            VkDescriptorBufferInfo bufferInfo{
                    .buffer = buffer->buffer_->GetNative(),
                    .offset = buffer->offset_,
                    .range  = buffer->size_,
            };

            VkWriteDescriptorSet descriptorWrite{
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet = vkDescriptorSet_,
                    .dstBinding = buffer->GetBindingPoint(),
                    .dstArrayElement = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .descriptorCount = 1,
                    .pBufferInfo = &bufferInfo,
                    .pImageInfo = nullptr, // optional
                    .pTexelBufferView = nullptr, // optional

            };
            vkUpdateDescriptorSets(vkDevice_, 1, &descriptorWrite, 0, nullptr);
        }
        else if (resource->GetResourceType() == BindingResourceType::COMBINED_SAMPLER_TEXTUREVIEW)
        {
            auto* vkRes = (const VKBindingCombined*)resource;

            VkDescriptorImageInfo imageInfo = {
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    .imageView = vkRes->textureView_->GetNative(),
                    .sampler = vkRes->sampler_->GetNative(),
            };

            VkWriteDescriptorSet descriptorWrite{
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet = vkDescriptorSet_,
                    .dstBinding = vkRes->GetBindingPoint(),
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
}

void VKBindGroup::BindToCommandBuffer(VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout) const
{
    vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &vkDescriptorSet_, 0, nullptr);
}

NS_RHI_END