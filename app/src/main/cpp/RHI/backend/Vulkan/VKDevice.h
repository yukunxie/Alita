//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_VKDEVICE_H
#define ALITA_VKDEVICE_H

#include "../../include/RHI.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <vector>
#include <map>

NS_RHI_BEGIN

class VKBuffer;
class VKGraphicPipeline;
class VKShader;
class VKQueue;

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

    VkDevice GetDevice() const {return vkDevice_;}

    VkPhysicalDevice GetPhysicalDevice() const {return vkPhysicalDevice_;}

    VkQueue GetQueue() const {return vkQueue_;}

    VkSwapchainKHR GetVkSwapChain() const {return vkSwapchain_;}

    VkExtent2D GetSwapChainExtent2D() const {return vkSwapchainExtent_;}

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkDescriptorPool GetDescriptorPool() {return vkDescriptorPool_;}

    std::uint32_t GetGraphicQueueFamilyIndex() const {return graphicQueueFamilyIndex_;}

    static void SetImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout,
                        VkPipelineStageFlags srcStages,
                        VkPipelineStageFlags destStages);

    VkCommandBuffer GetCommandBuffer() const {return commandBuffers_[imageIndex_];}

    const QueueFamilyIndices& GetQueueFamilyIndices() const {return queueFamilyIndices_;}

    std::uint32_t GetNextImageIndex();

    VkSemaphore  GetRenderFinishedSemaphore() const {return vkRenderFinishedSemaphore_;};

public:
    virtual Buffer* CreateBuffer(BufferUsageFlagBits usageFlagBits
            , SharingMode sharingMode
            , std::uint32_t sizeOfBytes
            , const void* data) override;

    virtual void WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size) override;

    virtual GraphicPipeline* CreateGraphicPipeline(const GraphicPipelineCreateInfo& graphicPipelineCreateInfo) override;

    virtual Shader* CreateShader(const std::vector<std::uint8_t>& shaderSource) override;

    virtual RenderPass* CreateRenderPass(const RenderPassCreateInfo& createInfo) override;

    virtual Texture* CreateTexture(const ImageCreateInfo& imageCreateInfo) override;

    virtual Sampler* CreateSampler() override ;

    virtual TextureView* CreateTextureView(const Texture* texture) override ;

    virtual BindGroupLayout* CreateBindGroupLayout(const DescriptorSetLayoutCreateInfo& layoutCreateInfo) override ;

    virtual BindGroup* CreateBindGroup(const BindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindResources) override ;

    virtual PipelineLayout* CreatePipelineLayout(const std::vector<BindGroupLayout*>& bindGroupLayouts) override ;

    virtual BindingResource* CreateBindingResourceBuffer(std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) override ;

    virtual BindingResource* CreateBindingResourceCombined(std::uint32_t bindingPoint, const TextureView* textureView, const Sampler* sampler) override ;

    virtual Queue* CreateQueue() override;

    virtual CommandEncoder* CreateCommandEncoder() override;

    virtual SwapChain* CreateSwapChain() override;

    virtual void WriteBindGroup(const BindGroup* bindGroup) override ;


    virtual void BeginRenderpass() override;

    virtual void EndRenderpass() override;

    virtual Viewport GetViewport() override {return viewport_;}

    virtual Scissor  GetScissor() override  {return scissor_;}

    virtual Queue* GetQueue() override;

private:
    void CreateInstance();
    void CreateSurface();
    void CreatePhysicDevice();
    void CreateDevice();
    void CreateSwapchain();
    void CreateVKQueue();
    void CreateCommandPool();
    void CreateDescriptorPool();
    void CreateCommandBuffers();
    void SetupSynchronizeObjects();


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
    VkCommandPool                   vkCommandPool_;
    VkDescriptorPool                vkDescriptorPool_;
    VkSurfaceKHR                    vkSurface_;
    VkSwapchainKHR                  vkSwapchain_;
    VkFormat                        vkSwapchainImageFormat_;
    VkExtent2D                      vkSwapchainExtent_;
    VkDebugReportCallbackEXT        vkDebugReportCallback_;
//    std::vector<VkImage>            swapChainImages_;
//    std::vector<VkImageView>        swapChainImageViews_;
//    std::vector<VkFramebuffer>      framebuffers_;
    std::vector<VkCommandBuffer>    commandBuffers_;
    QueueFamilyIndices              queueFamilyIndices_;

    VkSemaphore                     vkImageAvailableSemaphore_;
    VkSemaphore                     vkRenderFinishedSemaphore_;
    VkFence                         vkFence_;

    std::uint32_t                   imageIndex_ = 0;
    std::uint32_t                   graphicQueueFamilyIndex_ = 0;

    Viewport                        viewport_;
    Scissor                         scissor_;

    // VK*
    VKQueue*                        queue_      = nullptr;

    std::map<std::uint64_t, RenderPass*> renderPassCaches_;

};

NS_RHI_END


#endif //ALITA_VKDEVICE_H
