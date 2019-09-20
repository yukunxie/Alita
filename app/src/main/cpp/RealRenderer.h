//
// Created by realxie on 2019/3/23.
//

#ifndef VULKANDEMO_VULKANDEMO_H
#define VULKANDEMO_VULKANDEMO_H

#include <vector>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "vulkan_wrapper.h"

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

    bool initVulkanContext(struct android_app *app);

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

private:
    VkInstance createVKInstance(struct android_app *app);

    VkPhysicalDevice createVKGPU(struct android_app *app);

    VkDevice createVKDevice(struct android_app *app);

    VkPhysicalDevice createVKPhysicalDevice(VkInstance instance);

    VkSurfaceKHR createVKSurface(struct android_app *app);

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

private:
    VkInstance vkInstance_ = nullptr;
    VkPhysicalDevice vkGPU_ = nullptr;
    VkDevice vkDevice_ = nullptr;
    VkPhysicalDevice vkPhysicalDevice_ = nullptr;
    VkQueue vkQueue_ = nullptr;
    VkSurfaceKHR vkSurface_;
    VkSwapchainKHR vkSwapchain_;
    VkFence vkFence_;
    std::vector<VkImage> swapChainImages_;
    VkFormat swapChainImageFormat_;
    VkExtent2D swapChainExtent_;
    std::vector<VkImageView> swapChainImageViews_;

    VkRenderPass renderPass_;
    VkPipelineLayout pipelineLayout_;
    std::vector<VkFramebuffer> swapChainFramebuffers_;
    VkCommandPool commandPool_;
    std::vector<VkCommandBuffer> commandBuffers_;

    VkSemaphore imageAvailableSemaphore_;
    VkSemaphore renderFinishedSemaphore_;

    static RealRenderer *instance_;
};

bool initVulkanInstance(struct android_app *app);

#endif //VULKANDEMO_VULKANDEMO_H
