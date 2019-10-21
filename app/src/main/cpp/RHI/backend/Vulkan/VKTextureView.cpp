//
// Created by realxie on 2019-10-10.
//

#include "VKTextureView.h"

NS_RHI_BEGIN

VKTextureView::VKTextureView(VKDevice* device, const VKTexture* vkTexture)
{
    vkDevice_ = device->GetDevice();

    textureSize_ = vkTexture->GetTextureSize();

    VkImageViewCreateInfo viewInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = vkTexture->GetNative(),
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = vkTexture->GetFormat(),
            .components = {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B,
                    .a = VK_COMPONENT_SWIZZLE_A,
            },
            .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
            },
            .flags = 0,
    };
    CALL_VK(vkCreateImageView(vkDevice_, &viewInfo, nullptr, &vkImageView_));
}

VKTextureView::VKTextureView(VKDevice* device, const VkImageViewCreateInfo& imageViewCreateInfo, const Extent3D& textureSize)
{
    vkDevice_ = device->GetDevice();
    textureSize_ = textureSize;
    CALL_VK(vkCreateImageView(vkDevice_, &imageViewCreateInfo, nullptr, &vkImageView_));
}

VKTextureView::~VKTextureView()
{
    if (vkImageView_)
    {
        vkDestroyImageView(vkDevice_, vkImageView_, nullptr);
    }
}

NS_RHI_END
