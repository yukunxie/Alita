//
// Created by realxie on 2019-10-07.
//

#ifndef ALITA_VKTEXTURE_H
#define ALITA_VKTEXTURE_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/Texture.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKTexture : public Texture
{
public:
    VKTexture(VKDevice* device, const ImageCreateInfo& imageCreateInfo);
    virtual ~VKTexture();

    VkImage GetNative() const {return vkImage_;}

    VkFormat GetFormat() const {return vkFormat_;}

    Extent3D GetTextureSize() const {return textureSize_;}

private:
    void SetImageLayout(const VKDevice* device);

private:
    VkDevice        vkDevice_       = nullptr;
    VkImage         vkImage_        = 0L;
    VkDeviceMemory  vkDeviceMemory_ = 0L;
    VkFormat        vkFormat_       = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;

    Extent3D        textureSize_ ;
};

NS_RHI_END


#endif //ALITA_VKTEXTURE_H
