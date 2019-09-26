//
// Created by realxie on 2019/3/23.
//

#include "RealRenderer.h"

#include <vector>
#include <array>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../drivers/vulkan/vulkan_wrapper.h"
#include "aux/AFileSystem.h"
#include "glm/vec3.hpp"
#include "../external/glm/gtx/closest_point.inl"
#include "../aux/AFileSystem.h"

// Android log function wrappers
static const char *kTAG = "VulkanDemo";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

// Vulkan call wrapper
#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "Tutorial ",               \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
  }

RealRenderer *RealRenderer::instance_ = nullptr;

RealRenderer *RealRenderer::getInstance()
{
    if (instance_) {
        return instance_;
    }
    instance_ = new RealRenderer();
    return instance_;
}

RealRenderer::RealRenderer()
{
    auto ret = InitVulkan();
    assert(ret != 0);
}

RealRenderer::~RealRenderer()
{
}

bool RealRenderer::initVulkanContext(struct android_app *app)
{
    if (app->window == nullptr || isReady()) {
        return false;
    }

    vkInstance_ = createVKInstance(app);
    vkSurface_ = createVKSurface(app);
    vkGPU_ = createVKGPU(app);
    vkDevice_ = createVKDevice(app);
    vkPhysicalDevice_ = createVKPhysicalDevice(vkInstance_);
    vkSwapchain_ = createVKSwapChain(vkDevice_, vkPhysicalDevice_, vkSurface_);
    vkQueue_ = createVKQueue(vkDevice_, vkPhysicalDevice_, vkSurface_);

    VkFenceCreateInfo fenceCreateInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = nullptr, .flags = 0,};
    vkCreateFence(vkDevice_, &fenceCreateInfo, nullptr, &vkFence_);

    setupSwapchainEnv();
    createImageViews();
    createSemaphores();
    createRenderPass();
    createFramebuffers();
    createCommandPool();

    createShaderModule();

    createVertexBuffer();

    createCommandBuffers();


    return true;
}

void RealRenderer::setupSwapchainEnv()
{
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(vkDevice_, vkSwapchain_, &imageCount, nullptr);
    swapChainImages_.resize(imageCount);
    vkGetSwapchainImagesKHR(vkDevice_, vkSwapchain_, &imageCount, swapChainImages_.data());

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vkPhysicalDevice_, vkSurface_);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    swapChainImageFormat_ = surfaceFormat.format;
    swapChainExtent_ = extent;
}

VkInstance RealRenderer::createVKInstance(struct android_app *app)
{
    VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pNext = nullptr, .apiVersion = VK_MAKE_VERSION(
            1, 0, 0), .applicationVersion = VK_MAKE_VERSION(1, 0,
                                                            0), .engineVersion = VK_MAKE_VERSION(1,
                                                                                                 0,
                                                                                                 0), .pApplicationName = "Realxie's vulkan demo", .pEngineName = "demo",};

    // prepare necessary extensions: Vulkan on Android need these to function
    std::vector<const char *> instanceExt;
    instanceExt.push_back("VK_KHR_surface");
    instanceExt.push_back("VK_KHR_android_surface");

    // Create the Vulkan instance
    VkInstanceCreateInfo instanceCreateInfo{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, .pNext = nullptr, .pApplicationInfo = &appInfo, .enabledExtensionCount = static_cast<uint32_t>(instanceExt.size()), .ppEnabledExtensionNames = instanceExt.data(), .enabledLayerCount = 0, .ppEnabledLayerNames = nullptr,};

    VkInstance instance;
    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));

    return instance;
}

VkPhysicalDevice RealRenderer::createVKGPU(struct android_app *app)
{
    uint32_t gpuCount = 0;
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, nullptr));
    VkPhysicalDevice tmpGpus[10] = {nullptr};
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, tmpGpus));
    return tmpGpus[0];  // Pick up the first GPU Devicereturn true;
}

VkDevice RealRenderer::createVKDevice(struct android_app *app)
{
    // check for vulkan info on this GPU device
    VkPhysicalDeviceProperties gpuProperties;
    vkGetPhysicalDeviceProperties(vkGPU_, &gpuProperties);
    LOGI("Vulkan Physical Device Name: %s", gpuProperties.deviceName);
    LOGI("Vulkan Physical Device Info: apiVersion: %x \n\t driverVersion: %x",
         gpuProperties.apiVersion, gpuProperties.driverVersion);
    LOGI("API Version Supported: %d.%d.%d", VK_VERSION_MAJOR(gpuProperties.apiVersion),
         VK_VERSION_MINOR(gpuProperties.apiVersion), VK_VERSION_PATCH(gpuProperties.apiVersion));

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkGPU_, vkSurface_, &surfaceCapabilities);

    LOGI("Vulkan Surface Capabilities:\n");
    LOGI("\timage count: %u - %u\n", surfaceCapabilities.minImageCount,
         surfaceCapabilities.maxImageCount);
    LOGI("\tarray layers: %u\n", surfaceCapabilities.maxImageArrayLayers);
    LOGI("\timage size (now): %dx%d\n", surfaceCapabilities.currentExtent.width,
         surfaceCapabilities.currentExtent.height);
    LOGI("\timage size (extent): %dx%d - %dx%d\n", surfaceCapabilities.minImageExtent.width,
         surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.width,
         surfaceCapabilities.maxImageExtent.height);
    LOGI("\tusage: %x\n", surfaceCapabilities.supportedUsageFlags);
    LOGI("\tcurrent transform: %u\n", surfaceCapabilities.currentTransform);
    LOGI("\tallowed transforms: %x\n", surfaceCapabilities.supportedTransforms);
    LOGI("\tcomposite alpha flags: %u\n", surfaceCapabilities.currentTransform);

    // Find a GFX queue family
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(vkGPU_, &queueFamilyCount, nullptr);
    assert(queueFamilyCount);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkGPU_, &queueFamilyCount,
                                             queueFamilyProperties.data());

    uint32_t queueFamilyIndex;
    for (queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }
    assert(queueFamilyIndex < queueFamilyCount);

    float priorities[] = {1.0f,};
    VkDeviceQueueCreateInfo queueCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .pNext = nullptr, .flags = 0, .queueCount = 1, .queueFamilyIndex = queueFamilyIndex, .pQueuePriorities = priorities,};

    // prepare necessary extensions: Vulkan on Android need these to function
    std::vector<const char *> deviceExt;
    deviceExt.push_back("VK_KHR_swapchain");

    VkDeviceCreateInfo deviceCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .pNext = nullptr, .queueCreateInfoCount = 1, .pQueueCreateInfos = &queueCreateInfo, .enabledLayerCount = 0, .ppEnabledLayerNames = nullptr, .enabledExtensionCount = static_cast<uint32_t>(deviceExt.size()), .ppEnabledExtensionNames = deviceExt.data(), .pEnabledFeatures = nullptr,};

    VkDevice device;
    CALL_VK(vkCreateDevice(vkGPU_, &deviceCreateInfo, nullptr, &device));

    return device;
}

bool RealRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
}

VkPhysicalDevice RealRenderer::createVKPhysicalDevice(VkInstance instance)
{
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto &device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    return physicalDevice;
}

VkSurfaceKHR RealRenderer::createVKSurface(struct android_app *app)
{
    // if we create a surface, we need the surface extension
    VkAndroidSurfaceCreateInfoKHR createInfo{.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR, .pNext = nullptr, .flags = 0, .window = app->window};
    VkSurfaceKHR surface;
    vkCreateAndroidSurfaceKHR(vkInstance_, &createInfo, nullptr, &surface);
    return surface;
}

VkSurfaceFormatKHR
RealRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR
RealRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }
    return bestMode;
}

VkExtent2D RealRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {4096, 4096};
        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width,
                                               actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height,
                                                actualExtent.height));
        return actualExtent;
    }
}

SwapChainSupportDetails
RealRenderer::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    // Capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    //formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    // presentMode
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                                  details.presentModes.data());
    }
    return details;
}

QueueFamilyIndices RealRenderer::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

VkSwapchainKHR RealRenderer::createVKSwapChain(VkDevice device, VkPhysicalDevice physicalDevice,
                                               VkSurfaceKHR surface)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vkPhysicalDevice_, vkSurface_);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily,
                                     (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional   createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    VkSwapchainKHR swapChain;  // 声明

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    return swapChain;
}

VkQueue
RealRenderer::createVKQueue(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    VkQueue queue;
    vkGetDeviceQueue(device, indices.presentFamily, 0, &queue);
    return queue;
}

void RealRenderer::createImageViews()
{
    swapChainImageViews_.resize(swapChainImages_.size());

    for (size_t i = 0; i < swapChainImages_.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages_[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat_;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vkDevice_, &createInfo, nullptr, &swapChainImageViews_[i]) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

void RealRenderer::createRenderPass()
{
    VkAttachmentDescription attachmentDescriptions = {
            .format = swapChainImageFormat_,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colourReference = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpassDescription = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .flags = 0, .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colourReference,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
    };

    VkRenderPassCreateInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr, .attachmentCount = 1,
            .pAttachments = &attachmentDescriptions,
            .subpassCount = 1,
            .pSubpasses = &subpassDescription,
            .dependencyCount = 0,
            .pDependencies = nullptr
    };

    if (auto code = vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_); code !=
                                                                                           VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void RealRenderer::createFramebuffers()
{
    swapChainFramebuffers_.resize(swapChainImageViews_.size());

    for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews_[i]};
        VkFramebufferCreateInfo framebufferInfo {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .renderPass = vkRenderPass_,
                .layers = 1,
                .attachmentCount = 1,  // 2 if using depth
                .pAttachments = attachments,
                .width = static_cast<uint32_t>(swapChainExtent_.width),
                .height = static_cast<uint32_t>(swapChainExtent_.height)
        };

        if (vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void RealRenderer::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(vkPhysicalDevice_, vkSurface_);
    VkCommandPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = 0
    };

    if (auto code = vkCreateCommandPool(vkDevice_, &poolInfo, nullptr, &vkCommandPool_); code !=
                                                                                       VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

/*
 * setImageLayout():
 *    Helper function to transition color buffer layout
 */
void setImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldImageLayout,
                    VkImageLayout newImageLayout, VkPipelineStageFlags srcStages,
                    VkPipelineStageFlags destStages)
{
    VkImageMemoryBarrier imageMemoryBarrier = {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, .pNext = NULL, .srcAccessMask = 0, .dstAccessMask = 0, .oldLayout = oldImageLayout, .newLayout = newImageLayout, .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, .image = image, .subresourceRange =
            {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1,},};

    switch (oldImageLayout) {
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        default:
            break;
    }

    switch (newImageLayout) {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        default:
            break;
    }

    vkCmdPipelineBarrier(cmdBuffer, srcStages, destStages, 0, 0, NULL, 0, NULL, 1,
                         &imageMemoryBarrier);
}

void RealRenderer::createCommandBuffers()
{
    commandBuffers_.resize(swapChainFramebuffers_.size());

    //    VkCommandBufferAllocateInfo allocInfo = {};
    //    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    //    allocInfo.commandPool = vkCommandPool_;
    //    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    //    allocInfo.commandBufferCount = (uint32_t) commandBuffers_.size();
    //    if (vkAllocateCommandBuffers(vkDevice_, &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
    //        throw std::runtime_error("failed to allocate command buffers!");
    //    }

    for (size_t i = 0; i < commandBuffers_.size(); i++) {
        VkCommandBufferAllocateInfo cmdBufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = vkCommandPool_,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        if (auto code = vkAllocateCommandBuffers(vkDevice_, &cmdBufferCreateInfo, &commandBuffers_[i]); code != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        VkCommandBufferBeginInfo cmdBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
        };
        CALL_VK(vkBeginCommandBuffer(commandBuffers_[i], &cmdBufferBeginInfo));

        // transition the display image to color attachment layout
        setImageLayout(commandBuffers_[i],
                swapChainImages_[i],
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        // Now we start a renderpass. Any draw command has to be recorded in a
        // renderpass
        VkClearValue clearVals{
                .color.float32[0] = 1.0f,
                .color.float32[1] = 0.34f,
                .color.float32[2] = 0.90f,
                .color.float32[3] = 1.0f
        };

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = vkRenderPass_,
            .framebuffer = swapChainFramebuffers_[i],
            .renderArea = {.offset = {.x = 0, .y = 0 },.extent = swapChainExtent_},
            .clearValueCount = 1,
            .pClearValues = &clearVals
        };

        vkCmdBeginRenderPass(commandBuffers_[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

//        // draw some commands
//        {
//            vkCmdBindPipeline(commandBuffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);
//            VkDeviceSize offset = 0;
//            vkCmdBindVertexBuffers(commandBuffers_[i], 0, 1, &vkVertexBuffer_, &offset);
//            VKAPI_CALL(vkCmdDraw(commandBuffers_[i], 3, 1, 0, 0));
//        }

        vkCmdEndRenderPass(commandBuffers_[i]);

        // transition back to swapchain image to PRESENT_SRC_KHR
        CALL_VK(vkEndCommandBuffer(commandBuffers_[i]));
    }
}

void RealRenderer::createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore_) !=
        VK_SUCCESS ||
        vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphore_) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}

void RealRenderer::createShaderModule()
{
    VkShaderModule vertShaderModule;
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        const TData& data = AFileSystem::getInstance()->readData("shaders/shader.vert.spv");
        createInfo.codeSize = data.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    VkShaderModule fragShaderModule;
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        const TData& data = AFileSystem::getInstance()->readData("shaders/shader.frag.spv");
        createInfo.codeSize = data.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertShaderModule,
            .pName = "main",
    };

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragShaderModule,
            .pName = "main",
    };

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkVertexInputBindingDescription bindingDescription = {
            .binding = 0,
            .stride  = sizeof(TVertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {
            VkVertexInputAttributeDescription {
                    .binding = 0,
                    .location = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(TVertex, pos)
            },
            VkVertexInputAttributeDescription {
                    .binding = 0,
                    .location = 1,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset = offsetof(TVertex, color)
            }
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .vertexBindingDescriptionCount = 1,
            .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributeDescriptions.size()),
            .pVertexBindingDescriptions = &bindingDescription,
            .pVertexAttributeDescriptions = attributeDescriptions.data()
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
    };

    VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)swapChainExtent_.width,
            .height = (float)swapChainExtent_.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
    };

    VkRect2D scissor = {
            .offset = {0, 0},
            .extent = swapChainExtent_,
    };

    VkPipelineViewportStateCreateInfo viewportState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .lineWidth = 1.0f,
            .cullMode = VK_CULL_MODE_NONE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,    // Optional
            .depthBiasClamp = 0.0f,             // Optional
            .depthBiasSlopeFactor = 0.0f,       // Optional
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f, // Optional
            .pSampleMask = nullptr, // Optional
            .alphaToCoverageEnable = VK_FALSE, // Optional
            .alphaToOneEnable = VK_FALSE, // Optional
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE,

            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
            .colorBlendOp = VK_BLEND_OP_ADD, // Optional
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
            .alphaBlendOp = VK_BLEND_OP_ADD, // Optional

//            .blendEnable = VK_TRUE,
//            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
//            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
//            .colorBlendOp = VK_BLEND_OP_ADD,
//            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
//            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
//            .alphaBlendOp = VK_BLEND_OP_ADD,
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY, // Optional
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants[0] = 0.0f, // Optional
            .blendConstants[1] = 0.0f, // Optional
            .blendConstants[2] = 0.0f, // Optional
            .blendConstants[3] = 0.0f, // Optional
    };

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

    VkPipelineDynamicStateCreateInfo dynamicState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = 2,
            .pDynamicStates = dynamicStates,
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0, // Optional
            .pSetLayouts = nullptr, // Optional
            .pushConstantRangeCount = 0, // Optional
            .pPushConstantRanges = nullptr, // Optional
    };

    if (vkCreatePipelineLayout(vkDevice_, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = 2,
            .pStages = shaderStages,
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            pipelineInfo.pDepthStencilState = nullptr, // Optional
            .pColorBlendState = &colorBlending,
            .pDynamicState = nullptr, // Optional

            .layout = vkPipelineLayout_,
            .renderPass = vkRenderPass_,
            .subpass = 0,

            .basePipelineHandle = VK_NULL_HANDLE, // Optional
            .basePipelineIndex = -1, // Optional
    };

    if (vkCreateGraphicsPipelines(vkDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!"); }
}

void RealRenderer::createVertexBuffer()
{
    const std::vector<TVertex> vertices = {{{0.0f, -.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    // TODO define a class member
    uint32_t queueFamilyIndex_ = 0;

    VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = sizeof(vertices[0]) * vertices.size(),
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .pQueueFamilyIndices = &queueFamilyIndex_,
            .queueFamilyIndexCount = 1,
    };

    if (vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &vkVertexBuffer_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    // Allocate heap memory for vertex buffer

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice_, vkVertexBuffer_, &memRequirements);

    std::uint32_t memTypeMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VkMemoryAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memTypeMask)
    };

    if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &vkVertexBufferMemory_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    void* data;
    VKAPI_CALL(vkMapMemory(vkDevice_, vkVertexBufferMemory_, 0, bufferInfo.size, 0, &data));
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(vkDevice_, vkVertexBufferMemory_);
    VKAPI_CALL(vkBindBufferMemory(vkDevice_, vkVertexBuffer_, vkVertexBufferMemory_, 0));
}

uint32_t RealRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice_, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void RealRenderer::drawFrame()
{
    uint32_t imageIndex;
    VKAPI_CALL(vkAcquireNextImageKHR(vkDevice_, vkSwapchain_, std::numeric_limits<uint64_t>::max(),
                          vkImageAvailableSemaphore_, VK_NULL_HANDLE, &imageIndex));

    vkWaitForFences(vkDevice_, 1, &vkFence_, VK_TRUE, std::numeric_limits<uint64_t>::max());

    VkCommandBufferBeginInfo cmdBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
    };
    CALL_VK(vkBeginCommandBuffer(commandBuffers_[imageIndex], &cmdBufferBeginInfo));

    // transition the display image to color attachment layout
    setImageLayout(commandBuffers_[imageIndex],
                   swapChainImages_[imageIndex],
                   VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                   VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    VkClearValue clearVals{
            .color.float32[0] = 0.0f,
            .color.float32[1] = 1.0f,
            .color.float32[2] = 0.0f,
            .color.float32[3] = 1.0f
    };

    VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = vkRenderPass_,
            .framebuffer = swapChainFramebuffers_[imageIndex],
            .renderArea = {.offset = {.x = 0, .y = 0 },.extent = swapChainExtent_},
            .clearValueCount = 1,
            .pClearValues = &clearVals
    };

    vkCmdBeginRenderPass(commandBuffers_[imageIndex], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // draw some commands
    {
        vkCmdBindPipeline(commandBuffers_[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);
        VkBuffer vertexBuffers[] = {vkVertexBuffer_};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers_[imageIndex], 0, 1, vertexBuffers, offsets);

        VKAPI_CALL(vkCmdDraw(commandBuffers_[imageIndex], 3, 1, 0, 0));
    }

    vkCmdEndRenderPass(commandBuffers_[imageIndex]);

    CALL_VK(vkEndCommandBuffer(commandBuffers_[imageIndex]));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &vkImageAvailableSemaphore_;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers_[imageIndex];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &vkRenderFinishedSemaphore_;

    vkResetFences(vkDevice_, 1, &vkFence_);
    if (auto code = vkQueueSubmit(vkQueue_, 1, &submitInfo, vkFence_); code != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    LOGI("Drawing frames......");

    VkResult result;
    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .swapchainCount = 1,
        .pSwapchains = &vkSwapchain_,
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
