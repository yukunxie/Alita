//
// Created by realxie on 2019-10-10.
//

#include "VKBindGroup.h"


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

NS_RHI_END