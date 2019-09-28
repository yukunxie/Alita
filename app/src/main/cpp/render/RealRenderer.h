//
// Created by realxie on 2019/3/23.
//

#ifndef VULKANDEMO_VULKANDEMO_H
#define VULKANDEMO_VULKANDEMO_H

#include <vector>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "types/Types.h"
#include "aux/Aux.h"
#include "../drivers/vulkan/vulkan_wrapper.h"

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

class RealRenderer
{
public:
    virtual ~RealRenderer();

    static RealRenderer *getInstance();

    inline bool isReady()
    { return vkDevice_ != nullptr; }

    bool initVulkanContext(ANativeWindow* app);

    void drawFrame();

protected:
    RealRenderer();

    VkSurfaceFormatKHR
    chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR
    chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    bool isDeviceSuitable(VkPhysicalDevice device);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    VkInstance createVKInstance();

    VkPhysicalDevice createVKGPU();

    VkDevice createVKDevice();

    VkPhysicalDevice createVKPhysicalDevice(VkInstance instance);

    VkSurfaceKHR createVKSurface(ANativeWindow* window);

    VkSwapchainKHR
    createVKSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkQueue createVKQueue(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    void setupSwapchainEnv();

    void createImageViews();

    void createRenderPass();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSemaphores();

    void createShaderModule();

    void createVertexBuffer();

private:
    VkInstance vkInstance_ = nullptr;
    VkPhysicalDevice vkGPU_ = nullptr;
    VkDevice vkDevice_ = nullptr;
    VkPhysicalDevice vkPhysicalDevice_ = nullptr;
    VkQueue vkQueue_ = nullptr;
    VkSurfaceKHR vkSurface_;
    VkSwapchainKHR vkSwapchain_;
    VkFence vkFence_;
    VkFormat swapChainImageFormat_;
    VkExtent2D swapChainExtent_;
    VkRenderPass vkRenderPass_;
    VkPipeline vkGraphicsPipeline_;
    VkPipelineLayout vkPipelineLayout_;
    VkSemaphore vkImageAvailableSemaphore_;
    VkSemaphore vkRenderFinishedSemaphore_;
    VkCommandPool vkCommandPool_;

    //
    VkBuffer vkVertexBuffer_;
    VkDeviceMemory vkVertexBufferMemory_;

    std::vector<VkImage> swapChainImages_;
    std::vector<VkImageView> swapChainImageViews_;
    std::vector<VkFramebuffer> swapChainFramebuffers_;
    std::vector<VkCommandBuffer> commandBuffers_;

    static RealRenderer *instance_;
};
#endif //VULKANDEMO_VULKANDEMO_H
