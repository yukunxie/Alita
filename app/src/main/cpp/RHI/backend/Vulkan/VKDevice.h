//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_VKDEVICE_H
#define ALITA_VKDEVICE_H

#include "../../include/RHI.h"
#include "../../include/xxhash64.h"
#include "drivers/vulkan/vulkan_wrapper.h"
#include "VKTypes.h"

#include <vulkan/vulkan.h>
#include <android/native_window.h>

#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <array>

NS_RHI_BEGIN

class VKBuffer;
class VKRenderPipeline;
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

struct RenderPassCacheQuery
{
    void SetColor(uint32_t index, TextureFormat format, LoadOp loadOp)
    {
        colorFormats[index] = format;
        colorLoadOp[index]  = loadOp;
        colorMask.set(index, 1);
    }

    void SetDepthStencil(TextureFormat format, LoadOp depthLoadOp, LoadOp stencilLoadOp)
    {
        this->hasDepthStencil = true;
        this->depthStencilFormat = format;
        this->depthLoadOp = depthLoadOp;
        this->stencilLoadOp = stencilLoadOp;
    }

    bool operator == (const RenderPassCacheQuery& other) const
    {
        bool ret = true;
        ret &= hasDepthStencil == other.hasDepthStencil;
        ret &= this->depthStencilFormat == other.depthStencilFormat;
        ret &= this->depthLoadOp == other.depthLoadOp;
        ret &= this->stencilLoadOp == other.stencilLoadOp;
        ret &= memcmp(colorFormats.data(), other.colorFormats.data(), sizeof(colorFormats[0]) * colorFormats.size());
        ret &= memcmp(colorLoadOp.data(), other.colorLoadOp.data(), sizeof(colorLoadOp[0]) * colorLoadOp.size());
        return ret;
    }

    size_t operator()() const
    {
        size_t byteSize = 0;
        byteSize += sizeof(this->depthStencilFormat);
        byteSize += sizeof(this->depthLoadOp);
        byteSize += sizeof(this->stencilLoadOp);
        byteSize += sizeof(colorFormats[0]) * colorFormats.size();
        byteSize += sizeof(colorLoadOp[0]) * colorLoadOp.size();
        byteSize += sizeof(this->hasDepthStencil);

        std::vector<std::uint8_t> buffer(byteSize, 0);
        std::uint8_t* pData = buffer.data();

        memcpy(pData, colorFormats.data(), sizeof(colorFormats[0]) * colorFormats.size());
        pData += sizeof(colorFormats[0]) * colorFormats.size();

        memcpy(pData, colorLoadOp.data(), sizeof(colorLoadOp[0]) * colorLoadOp.size());
        pData += sizeof(colorLoadOp[0]) * colorLoadOp.size();

        memcpy(pData, &depthStencilFormat, sizeof(depthStencilFormat));
        pData += sizeof(depthStencilFormat);

        memcpy(pData, &depthLoadOp, sizeof(depthLoadOp));
        pData += sizeof(depthLoadOp);

        memcpy(pData, &stencilLoadOp, sizeof(stencilLoadOp));
        pData += sizeof(stencilLoadOp);

        memcpy(pData, &hasDepthStencil, sizeof(hasDepthStencil));

        static XXHash64 _hashFunc (0x21378732);

        return (size_t)(_hashFunc.hash(pData, byteSize, 0));
    }

    // member data
    std::bitset<kMaxColorAttachments> colorMask;
    std::array<TextureFormat, kMaxColorAttachments> colorFormats;
    std::array<LoadOp, kMaxColorAttachments> colorLoadOp;
    TextureFormat depthStencilFormat;
    LoadOp depthLoadOp;
    LoadOp stencilLoadOp;
    bool hasDepthStencil = false;
};

struct RenderPassCacheQueryFuncs
{
    size_t operator()(const RenderPassCacheQuery& query) const
    {
        return query();
    }

    bool operator()(const RenderPassCacheQuery& a, const RenderPassCacheQuery& b) const
    {
        return a == b;
    }
};

typedef std::unordered_map<RenderPassCacheQuery, RenderPass*, RenderPassCacheQueryFuncs, RenderPassCacheQueryFuncs> RenderPassCache;

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

    VkCommandPool GetCommandBufferPool() const {return vkCommandPool_;}

    const QueueFamilyIndices& GetQueueFamilyIndices() const {return queueFamilyIndices_;}

    void AddWaitingSemaphore(VkSemaphore semaphore) {waitingSemaphores_.push_back(semaphore);}

    const std::vector<VkSemaphore>& GetWaitingSemaphores() {return waitingSemaphores_;}

    void ClearWaitingSemaphores() {waitingSemaphores_.clear();}

    const VkFormat GetPresentColorFormat() const {return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;}

    RenderPass* GetOrCreateRenderPass(const RenderPassCacheQuery& query);

public:
    virtual Buffer* CreateBuffer(const BufferDescriptor& descriptor) override;

    virtual void WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size) override;

    virtual RenderPipeline* CreateRenderPipeline(const RenderPipelineDescriptor& descriptor) override;

    virtual Shader* CreateShader(const std::vector<std::uint8_t>& shaderSource) override;

    virtual Texture* CreateTexture(const TextureDescriptor& descriptor) override ;

    virtual Sampler* CreateSampler() override ;

    virtual BindGroupLayout* CreateBindGroupLayout(const DescriptorSetLayoutCreateInfo& layoutCreateInfo) override ;

    virtual BindGroup* CreateBindGroup(const BindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindResources) override ;

    virtual PipelineLayout* CreatePipelineLayout(const std::vector<BindGroupLayout*>& bindGroupLayouts) override ;

    virtual BindingResource* CreateBindingResourceBuffer(std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) override ;

    virtual BindingResource* CreateBindingResourceCombined(std::uint32_t bindingPoint, const TextureView* textureView, const Sampler* sampler) override ;

    virtual Queue* CreateQueue() override;

    virtual CommandEncoder* CreateCommandEncoder() override;

    virtual SwapChain* CreateSwapChain() override;

    virtual void WriteBindGroup(const BindGroup* bindGroup) override ;

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
    QueueFamilyIndices              queueFamilyIndices_;

    std::uint32_t                   graphicQueueFamilyIndex_ = 0;

    Viewport                        viewport_;
    Scissor                         scissor_;

    // VK*
    VKQueue*                                queue_      = nullptr;
    std::vector<VkSemaphore>                waitingSemaphores_;

    RenderPassCache renderPassCache_;
};

NS_RHI_END


#endif //ALITA_VKDEVICE_H
