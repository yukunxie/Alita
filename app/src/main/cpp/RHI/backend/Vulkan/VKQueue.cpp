//
// Created by realxie on 2019-10-18.
//

#include "VKQueue.h"
#include "VKCommandBuffer.h"

NS_RHI_BEGIN

VKQueue::VKQueue(VKDevice* device)
    : device_(device)
{
    vkDevice_ = device->GetDevice();

    vkGetDeviceQueue(vkDevice_, device->GetQueueFamilyIndices().presentFamily, 0, &vkQueue_);

    VkFenceCreateInfo fenceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
    };
    CALL_VK(vkCreateFence(vkDevice_, &fenceCreateInfo, nullptr, &vkFence_));
}

VKQueue::~VKQueue()
{
    // TODO release vulkan resource
}

void VKQueue::Submit(CommandBuffer* commandBuffer)
{
    auto vkCommandBuffer = RHI_CAST(VKCommandBuffer*, commandBuffer)->GetNative();
    CALL_VK(vkEndCommandBuffer(vkCommandBuffer));

    auto vkSwapchain = device_->GetVkSwapChain();
    auto imageIndex  = device_->GetNextImageIndex();

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCommandBuffer;

    auto renderFinishedSemaphore = device_->GetRenderFinishedSemaphore();

    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

    CALL_VK(vkResetFences(vkDevice_, 1, &vkFence_));
    if (auto code = vkQueueSubmit(vkQueue_, 1, &submitInfo, vkFence_); code != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    CALL_VK(vkWaitForFences(vkDevice_, 1, &vkFence_, VK_TRUE, std::numeric_limits<uint64_t>::max()));

    VkResult result;
    VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &vkSwapchain,
            .pImageIndices = &imageIndex,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pResults = &result
    };

    if (auto code = vkQueuePresentKHR(vkQueue_, &presentInfo); code == VK_ERROR_OUT_OF_DATE_KHR ||
                                                               code == VK_SUBOPTIMAL_KHR)
    {
        // TODO: recreate swapchain
    }
    else if (code != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
    vkQueueWaitIdle(vkQueue_);
}

NS_RHI_END