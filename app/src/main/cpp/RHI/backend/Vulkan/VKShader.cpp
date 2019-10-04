//
// Created by realxie on 2019-10-04.
//

#include "VKShader.h"


NS_RHI_BEGIN

VKShader::VKShader(VKDevice* device, const std::vector<std::uint8_t>& shaderSource)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderSource.size();
    createInfo.pCode = reinterpret_cast<const std::uint32_t *>(shaderSource.data());

    if (vkCreateShaderModule(device->GetVulkanDevice(), &createInfo, nullptr, &vkShaderModule_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

VKShader::~VKShader()
{
    // TODO destory shader resource
}


NS_RHI_END