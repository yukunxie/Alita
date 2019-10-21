//
// Created by realxie on 2019-10-21.
//

#include "VKSwapChain.h"

NS_RHI_BEGIN

VKSwapChain::VKSwapChain(VKDevice* device)
    : device_(device)
{
    auto vkDevice = device->GetDevice();
    
    std::vector<VkImage> swapChainImages;
    uint32_t imageCount = 0;

    vkGetSwapchainImagesKHR(vkDevice, device->GetVkSwapChain(), &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkDevice, device->GetVkSwapChain(), &imageCount, swapChainImages.data());

    // TODO configurable here
    auto vkSwapchainImageFormat = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;

    auto extent2D = device->GetSwapChainExtent2D();
    Extent3D extent3D {
        .width = extent2D.width,
        .height = extent2D.height,
        .depth = 1,
    };

    // setup swapChainImageViews_
    swapChainImageViews_.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = vkSwapchainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        auto textureImage = new VKTextureView(device, createInfo, extent3D);
        RHI_SAFE_RETAIN(textureImage);
        swapChainImageViews_[i] = textureImage;
    }
}

VKSwapChain::~VKSwapChain()
{
    for (auto textureView : swapChainImageViews_)
    {
        RHI_SAFE_RELEASE(textureView);
    }
    swapChainImageViews_.clear();
}

TextureView* VKSwapChain::GetCurrentTexture()
{
    auto imageIndex = device_->GetNextImageIndex();
    RHI_ASSERT(imageIndex >= 0 && imageIndex < swapChainImageViews_.size());
    return swapChainImageViews_[imageIndex];
}

NS_RHI_END
