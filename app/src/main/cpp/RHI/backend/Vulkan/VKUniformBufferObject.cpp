//
// Created by realxie on 2019-10-06.
//

#include "VKUniformBufferObject.h"

NS_RHI_BEGIN

VKUniformBufferObject::VKUniformBufferObject(VKDevice* device, const VKGraphicPipeline* graphicPipeline)
{
    VkDescriptorSetLayout layout = graphicPipeline->GetDescriptorSetLayout();
    VkDescriptorSetAllocateInfo allocInfo  {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorSetCount = 1,
            .pSetLayouts = &layout,
            .descriptorPool = device->GetDescriptorPool()
    };

    CALL_VK(vkAllocateDescriptorSets(device->GetVulkanDevice(), &allocInfo, &vkDescriptorSet_));
//    auto code = vkAllocateDescriptorSets(device->GetVulkanDevice(), &allocInfo, &vkDescriptorSet_);
//    RHI_ASSERT(code == VK_SUCCESS);
}

VKUniformBufferObject::~VKUniformBufferObject()
{
    // TODO release vulkan resource
}

NS_RHI_END
