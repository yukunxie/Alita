//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_VKSHADER_H
#define VULKANDEMO_VKSHADER_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/Shader.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKShader : public Shader
{
public:
    VKShader(VKDevice* device, const std::vector<std::uint8_t>& shaderSource);
    virtual ~VKShader();

    VkShaderModule  GetNative() {return vkShaderModule_;}

private:
    VkShaderModule vkShaderModule_  = 0;
};

NS_RHI_END

#endif //VULKANDEMO_VKSHADER_H
