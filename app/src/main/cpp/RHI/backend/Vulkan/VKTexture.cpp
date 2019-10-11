//
// Created by realxie on 2019-10-07.
//

#include "VKTexture.h"
#include "VKTypes.h"


NS_RHI_BEGIN

VKTexture::VKTexture(VKDevice* device, const ImageCreateInfo& imageCreateInfo)
{
    vkDevice_ = device->GetDevice();
    std::uint32_t queueFamilyIndex = device->GetGraphicQueueFamilyIndex();

    vkFormat_ = ToVkFormat(imageCreateInfo.format);

    // Check for linear supportability
    VkFormatProperties props;
    bool needBlit = true;
    vkGetPhysicalDeviceFormatProperties(device->GetPhysicalDevice(), ToVkFormat(imageCreateInfo.format), &props);
    assert((props.linearTilingFeatures | props.optimalTilingFeatures) &
           VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

    if (props.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
        // linear format supporting the required texture
        needBlit = false;
    }

    VkImageCreateInfo imageInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .format = ToVkFormat(imageCreateInfo.format),
            .imageType = ToVkImageType(imageCreateInfo.imageType),
            .extent = {
                    .width = imageCreateInfo.extent.width,
                    .height = imageCreateInfo.extent.height,
                    .depth = imageCreateInfo.extent.depth,
            },
            .mipLevels = imageCreateInfo.mipLevels,
            .arrayLayers = imageCreateInfo.arrayLayers,
            .tiling = ToVkImageTiling(imageCreateInfo.tiling),
            .initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED,
            .sharingMode =  ToVkSharingMode(imageCreateInfo.sharingMode),
            .usage =(needBlit ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_SAMPLED_BIT),
            .samples = ToVkSampleCountFlagBits(imageCreateInfo.samples),
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &queueFamilyIndex,
            .flags = 0,
    };

    CALL_VK(vkCreateImage(vkDevice_, &imageInfo, nullptr, &vkImage_));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkDevice_, vkImage_, &memRequirements);

    VkMemoryAllocateInfo allocInfo {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
    };

    CALL_VK(vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &vkDeviceMemory_));
    CALL_VK(vkBindImageMemory(vkDevice_, vkImage_, vkDeviceMemory_, 0));

    RHI_ASSERT(imageCreateInfo.format == Format::R8G8B8A8_UNORM);
    VkDeviceSize imageSize = imageCreateInfo.extent.width * imageCreateInfo.extent.height * 4;

    const VkImageSubresource subres = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = 0, .arrayLayer = 0,
    };
    VkSubresourceLayout layout;
    vkGetImageSubresourceLayout(vkDevice_, vkImage_, &subres,
                                &layout);

//    std::vector<uint8_t> tmpData(static_cast<size_t>(imageSize), 255);
    void* data;
    vkMapMemory(vkDevice_, vkDeviceMemory_, 0, imageSize, 0, &data);
    memcpy(data, imageCreateInfo.imageData, static_cast<size_t>(imageSize));
    vkUnmapMemory(vkDevice_, vkDeviceMemory_);

    SetImageLayout(device);

//    VkImageViewCreateInfo viewInfo {
//            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//            .image = vkImage_,
//            .viewType = VK_IMAGE_VIEW_TYPE_2D,
//            .format = ToVkFormat(imageCreateInfo.format),
//            .components = {
//                .r = VK_COMPONENT_SWIZZLE_R,
//                .g = VK_COMPONENT_SWIZZLE_G,
//                .b = VK_COMPONENT_SWIZZLE_B,
//                .a = VK_COMPONENT_SWIZZLE_A,
//            },
//            .subresourceRange = {
//                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//                .baseMipLevel = 0,
//                .levelCount = 1,
//                .baseArrayLayer = 0,
//                .layerCount = 1,
//            },
//            .flags = 0,
//    };
//    CALL_VK(vkCreateImageView(vkDevice_, &viewInfo, nullptr, &vkImageView_));
}

VKTexture::~VKTexture()
{
    // TODO release memory
}


void VKTexture::SetImageLayout(const VKDevice* device)
{
    VkCommandPoolCreateInfo cmdPoolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = device->GetGraphicQueueFamilyIndex(),
    };

    VkCommandPool cmdPool;
    CALL_VK(vkCreateCommandPool(vkDevice_, &cmdPoolCreateInfo, nullptr, &cmdPool));

    VkCommandBuffer gfxCmd;
    const VkCommandBufferAllocateInfo cmd = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = cmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
    };

    CALL_VK(vkAllocateCommandBuffers(vkDevice_, &cmd, &gfxCmd));
    VkCommandBufferBeginInfo cmd_buf_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr};
    CALL_VK(vkBeginCommandBuffer(gfxCmd, &cmd_buf_info));

    device->SetImageLayout(gfxCmd, vkImage_, VK_IMAGE_LAYOUT_UNDEFINED,
                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    CALL_VK(vkEndCommandBuffer(gfxCmd));
    VkFenceCreateInfo fenceInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    VkFence fence;
    CALL_VK(vkCreateFence(vkDevice_, &fenceInfo, nullptr, &fence));
    vkResetFences(vkDevice_, 1, &fence);

    VkSubmitInfo submitInfo = {
            .pNext = nullptr,
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &gfxCmd,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    CALL_VK(vkQueueSubmit(device->GetQueue(), 1, &submitInfo, fence));
    CALL_VK(vkWaitForFences(vkDevice_, 1, &fence, VK_TRUE, 100000000));

    vkDestroyFence(vkDevice_, fence, nullptr);
    vkFreeCommandBuffers(vkDevice_, cmdPool, 1, &gfxCmd);
    vkDestroyCommandPool(vkDevice_, cmdPool, nullptr);
}

NS_RHI_END