//
// Created by realxie on 2019-10-21.
//

#include "VKSwapChain.h"
#include "VKQueue.h"

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

    // Create a semaphore to sync swapchain
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    CALL_VK(vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore_));
}

VKSwapChain::~VKSwapChain()
{
    for (auto textureView : swapChainImageViews_)
    {
        RHI_SAFE_RELEASE(textureView);
    }
    swapChainImageViews_.clear();
    vkDestroySemaphore(device_->GetDevice(), vkImageAvailableSemaphore_, nullptr);
}

TextureView* VKSwapChain::GetCurrentTexture()
{
    CALL_VK(vkAcquireNextImageKHR(device_->GetDevice(), device_->GetVkSwapChain(), std::numeric_limits<uint64_t>::max(),
                                  vkImageAvailableSemaphore_, VK_NULL_HANDLE, &imageIndex_));
    device_->AddWaitingSemaphore(vkImageAvailableSemaphore_);
    return swapChainImageViews_[imageIndex_];
}

void VKSwapChain::Present(const Queue* queue)
{
    VkSwapchainKHR vkSwapchain = device_->GetVkSwapChain();
    VkResult result;
    VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &vkSwapchain,
            .pImageIndices = &imageIndex_,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pResults = &result
    };

    VkQueue vkQueue = RHI_CAST(const VKQueue*, queue)->GetNative();
    if (auto code = vkQueuePresentKHR(vkQueue, &presentInfo); code == VK_ERROR_OUT_OF_DATE_KHR ||
                                                               code == VK_SUBOPTIMAL_KHR)
    {
        // TODO: recreate swapchain
    }
    else if (code != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vkQueueWaitIdle(vkQueue);
}

NS_RHI_END
