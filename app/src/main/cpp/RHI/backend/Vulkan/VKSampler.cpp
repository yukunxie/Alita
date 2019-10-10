//
// Created by realxie on 2019-10-07.
//

#include "VKSampler.h"

NS_RHI_BEGIN

VKSampler::VKSampler(VKDevice* device)
{
    vkDevice_ = device->GetDevice();

    VkSamplerCreateInfo samplerInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .magFilter = VK_FILTER_NEAREST,
            .minFilter = VK_FILTER_NEAREST,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .maxAnisotropy = 1,
            .compareOp = VK_COMPARE_OP_NEVER,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
            .unnormalizedCoordinates = VK_FALSE,
    };

    CALL_VK(vkCreateSampler(vkDevice_, &samplerInfo, nullptr, &vkSampler_));
}

VKSampler::~VKSampler()
{
    vkDestroySampler(vkDevice_, vkSampler_, nullptr);
}

NS_RHI_END