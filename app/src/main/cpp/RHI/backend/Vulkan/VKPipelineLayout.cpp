//
// Created by realxie on 2019-10-10.
//

#include "VKPipelineLayout.h"

NS_RHI_BEGIN

VKPipelineLayout::VKPipelineLayout(VKDevice* device, const std::vector<BindGroupLayout*>& bindGroupLayouts)
{
    vkDevice_ = device->GetDevice();

    std::vector<VkDescriptorSetLayout> setLayouts;
    for (const BindGroupLayout* bindGroupLayout: bindGroupLayouts)
    {
        setLayouts.push_back(((VKBindGroupLayout*)bindGroupLayout)->GetNative());
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = (std::uint32_t)setLayouts.size(),
            .pSetLayouts = setLayouts.data(),
            .pushConstantRangeCount = 0, // Optional
            .pPushConstantRanges = nullptr, // Optional
    };

    CALL_VK(vkCreatePipelineLayout(vkDevice_, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_));
}

VKPipelineLayout::~VKPipelineLayout()
{

}

NS_RHI_END