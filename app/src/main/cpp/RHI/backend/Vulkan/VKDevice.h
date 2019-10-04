//
// Created by realxie on 2019-10-02.
//

#ifndef VULKANDEMO_VKDEVICE_H
#define VULKANDEMO_VKDEVICE_H

#include "../../include/Macros.h"
#include "../../include/Device.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>

NS_RHI_BEGIN

class VKBuffer;
class VKGraphicPipeline;
class VKShader;

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

    VkDevice GetVulkanDevice() {return vkDevice_;}
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkRenderPass GetRenderPass() {return vkRenderPass_;}

public:
    virtual BufferHnd CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data) override;
    virtual GraphicPipelineHnd CreateGraphicPipeline(const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader) override;
    virtual ShaderHnd CreateShader(const std::vector<std::uint8_t>& shaderSource) override ;
    virtual void BeginRenderpass() override;
    virtual void EndRenderpass() override;
    virtual void BindBuffer(BufferHnd buffer, std::uint32_t offset) override ;
    virtual void Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) override;
    virtual void Draw(std::uint32_t vertexCount, std::uint32_t firstVertex) override;
    virtual void BindGraphicPipeline(GraphicPipelineHnd graphicPipeline) override;
    virtual Viewport GetViewport() override {return viewport_;};

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

    VkSemaphore                     vkImageAvailableSemaphore_;
    VkSemaphore                     vkRenderFinishedSemaphore_;
    VkFence                         vkFence_;

    std::uint32_t                   imageIndex_ = 0;

    Viewport                        viewport_;

};

NS_RHI_END


#endif //VULKANDEMO_VKDEVICE_H
