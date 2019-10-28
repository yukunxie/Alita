//
// Created by realxie on 2019-10-10.
//

#include "VKBindGroup.h"
#include "VKBuffer.h"
#include "VKTextureView.h"
#include "VKSampler.h"


NS_RHI_BEGIN

bool VKBindGroup::Init(VKDevice* device, const BindGroupDescriptor &descriptor)
{
    vkDevice_ = device->GetDevice();
    vkDescriptorPool_ = device->GetDescriptorPool();
    
    VkDescriptorSetLayout setLayout = RHI_CAST(const VKBindGroupLayout*,
                                               descriptor.layout)->GetNative();
    VkDescriptorSetAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorSetCount = 1,
        .pSetLayouts = &setLayout,
        .descriptorPool = vkDescriptorPool_
    };
    
    CALL_VK(vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, &vkDescriptorSet_));
    
    for (const auto res : descriptor.bindings)
    {
        bindingResources_.push_back(res);
    }
    return true;
}

VKBindGroup* VKBindGroup::Create(VKDevice* device, const BindGroupDescriptor &descriptor)
{
    auto ret = new VKBindGroup();
    if (ret && ret->Init(device, descriptor))
    {
        RHI_SAFE_RETAIN(ret);
        return ret;
    }
    
    if (ret) delete ret;
    return nullptr;
}


VKBindGroup::~VKBindGroup()
{
    if (vkDescriptorSet_)
    {
        vkFreeDescriptorSets(vkDevice_, vkDescriptorPool_, 1, &vkDescriptorSet_);
    }
}

void VKBindGroup::WriteToGPU() const
{
    for (const BindGroupBinding &binding : bindingResources_)
    {
        auto resource = binding.resource;
        if (resource->GetResourceType() == BindingResourceType::BUFFER)
        {
            auto* buffer = RHI_CAST(const VKBindingBuffer*, resource);
            VkDescriptorBufferInfo bufferInfo{
                .buffer = buffer->buffer_->GetNative(),
                .offset = buffer->offset_,
                .range  = buffer->size_,
            };
            
            VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = vkDescriptorSet_,
                .dstBinding = binding.binding,
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
            auto* vkRes = RHI_CAST(const VKBindingCombined*, resource);
            
            VkDescriptorImageInfo imageInfo = {
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .imageView = vkRes->textureView_->GetNative(),
                .sampler = vkRes->sampler_->GetNative(),
            };
            
            VkWriteDescriptorSet descriptorWrite{
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = vkDescriptorSet_,
                .dstBinding = binding.binding,
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

void VKBindGroup::BindToCommandBuffer(std::uint32_t index, VkCommandBuffer vkCommandBuffer,
                                      VkPipelineLayout vkPipelineLayout) const
{
    vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout,
                            index, 1, &vkDescriptorSet_, 0, nullptr);
}

NS_RHI_END