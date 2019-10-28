//
// Created by realxie on 2019-10-04.
//

#include "VKShader.h"


NS_RHI_BEGIN

bool VKShader::Init(VKDevice* device, const ShaderModuleDescriptor& descriptor)
{
    vkDevice_ = device->GetDevice();
    
    if (descriptor.codeType == ShaderCodeType::BINARY)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = descriptor.binaryCode.size();
        createInfo.pCode = reinterpret_cast<const std::uint32_t *>(descriptor.binaryCode.data());
    
        CALL_VK(vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &vkShaderModule_));
        return true;
    }
    else
    {
        return false;
    }
}

VKShader* VKShader::Create(VKDevice* device, const ShaderModuleDescriptor& descriptor)
{
    auto ret = new VKShader();
    if (ret && ret->Init(device, descriptor))
    {
        RHI_SAFE_RETAIN(ret);
        return ret;
    }
    if (ret) delete ret;
    return nullptr;
}

VKShader::~VKShader()
{
    if (vkShaderModule_)
    {
        vkDestroyShaderModule(vkDevice_, vkShaderModule_, nullptr);
    }
}


NS_RHI_END