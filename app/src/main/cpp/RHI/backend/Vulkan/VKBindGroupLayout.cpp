//
// Created by realxie on 2019-10-10.
//

#include "VKBindGroupLayout.h"
#include "VKTypes.h"

NS_RHI_BEGIN

VKBindGroupLayout::VKBindGroupLayout(VKDevice* device, const DescriptorSetLayoutCreateInfo& layoutCreateInfo)
{
    vkDevice_ = device->GetDevice();
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
    for (const auto& binding : layoutCreateInfo.bindings)
    {
        VkDescriptorSetLayoutBinding layoutBinding {
                .binding = binding.binding,
                .descriptorType = ToVkDescriptorType(binding.descriptorType),
                .descriptorCount= 1,
                .stageFlags = ToVkShaderStageFlags(binding.stageFlags),
                .pImmutableSamplers = nullptr,
        };
        layoutBindings.push_back(layoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .bindingCount = (std::uint32_t)layoutBindings.size(),
            .pBindings = layoutBindings.data(),
    };

    CALL_VK(vkCreateDescriptorSetLayout(vkDevice_, &layoutInfo, nullptr, &vkBindGroupLayout_));
}

VKBindGroupLayout::~VKBindGroupLayout()
{
    // TODO release vulkan resource
}

NS_RHI_END
