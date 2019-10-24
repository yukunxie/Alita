//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKTEXTUREVIEW_H
#define ALITA_VKTEXTUREVIEW_H

#include "VKDevice.h"
#include "VKTexture.h"

#include "../../include/Macros.h"
#include "../../include/TextureView.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKTextureView final: public TextureView
{
public:
    VKTextureView(VKDevice* device, const VKTexture* vkTexture);
    VKTextureView(VKDevice* device, const VkImageViewCreateInfo& imageViewCreateInfo, const Extent3D& textureSize);
    virtual ~VKTextureView();

    VkImageView GetNative() const {return vkImageView_;}

    Extent3D GetTextureSize() const {return textureSize_;}

    virtual TextureFormat  GetFormat() const override {return textureFormat_;}

private:
    VkDevice        vkDevice_       = nullptr;
    VkImageView     vkImageView_    = 0L;
    Extent3D        textureSize_;

    VKTexture*      texture_        = nullptr;
    TextureFormat   textureFormat_;
};

NS_RHI_END


#endif //ALITA_VKTEXTUREVIEW_H
