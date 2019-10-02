//
// Created by realxie on 2019-10-02.
//

#ifndef VULKANDEMO_VKDEVICE_H
#define VULKANDEMO_VKDEVICE_H

#include "../../include/Macros.h"
#include "../../include/Device.h"
#include "../../../drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>

NS_RHI_BEGIN

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete()
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

class VKDevice : public Device
{
public:
    VKDevice(ANativeWindow *window);
    ~VKDevice();

public:
    virtual BufferHnd CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, void* data) override;
    virtual void CreateShaderModule(const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader) override;
    virtual void BeginRenderpass() override;
    virtual void EndRenderpass() override;

private:
    void CreateInstance();
    void CreateSurface();
    void CreatePhysicDevice();
    void CreateDevice();
    void CreateSwapchain();
    void CreateRenderPass();
    void CreateFramebuffers();
    void CreateQueue();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void SetupSynchronizeObjects();

    void SetImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout, VkPipelineStageFlags srcStages,
                        VkPipelineStageFlags destStages);

private:
    QueueFamilyIndices FindQueueFamilies();
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    ANativeWindow*                  nativeWindow_           = nullptr;
    VkInstance                      vkInstance_             = nullptr;
    VkDevice                        vkDevice_               = nullptr;
    VkQueue                         vkQueue_                = nullptr;
    VkPhysicalDevice                vkPhysicalDevice_       = nullptr;
    VkRenderPass                    vkRenderPass_;
    VkCommandPool                   vkCommandPool_;
    VkSurfaceKHR                    vkSurface_;
    VkSwapchainKHR                  vkSwapchain_;
    VkFormat                        vkSwapchainImageFormat_;
    VkExtent2D                      vkSwapchainExtent_;
    std::vector<VkImage>            swapChainImages_;
    std::vector<VkImageView>        swapChainImageViews_;
    std::vector<VkFramebuffer>      framebuffers_;
    std::vector<VkCommandBuffer>    commandBuffers_;
    QueueFamilyIndices              queueFamilyIndices_;

    VkPipeline                      vkGraphicsPipeline_; // tmp
    VkPipelineLayout                vkPipelineLayout_; // tmp

    VkSemaphore                     vkImageAvailableSemaphore_;
    VkSemaphore                     vkRenderFinishedSemaphore_;
    VkFence                         vkFence_;

};

NS_RHI_END


#endif //VULKANDEMO_VKDEVICE_H
