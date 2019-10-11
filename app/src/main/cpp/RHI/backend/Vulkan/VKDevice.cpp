//
// Created by realxie on 2019-10-02.
//

#include "VKDevice.h"
#include "VulkanMarcos.h"
#include "VKBuffer.h"
#include "VKGraphicPipeline.h"
#include "VKShader.h"
#include "VKRenderPass.h"
#include "VKTexture.h"
#include "VKSampler.h"
#include "VKTextureView.h"
#include "VKPipelineLayout.h"
#include "VKBindingResources.h"
#include "VKBindGroupLayout.h"
#include "VKBindGroup.h"

#include <vector>
#include <array>

NS_RHI_BEGIN

PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;

static VKAPI_ATTR VkBool32 VKAPI_CALL  DebugReportCallback(
        VkDebugReportFlagsEXT msgFlags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t srcObject, size_t location,
        int32_t msgCode, const char * pLayerPrefix,
        const char * pMsg, void * pUserData )
{
    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        __android_log_print(ANDROID_LOG_ERROR,
                            "AppName",
                            "ERROR: [%s] Code %i : %s",
                            pLayerPrefix, msgCode, pMsg);
    } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        __android_log_print(ANDROID_LOG_WARN,
                            "AppName",
                            "WARNING: [%s] Code %i : %s",
                            pLayerPrefix, msgCode, pMsg);
    } else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        __android_log_print(ANDROID_LOG_WARN,
                            "AppName",
                            "PERFORMANCE WARNING: [%s] Code %i : %s",
                            pLayerPrefix, msgCode, pMsg);
    } else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        __android_log_print(ANDROID_LOG_INFO,
                            "AppName", "INFO: [%s] Code %i : %s",
                            pLayerPrefix, msgCode, pMsg);
    } else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        __android_log_print(ANDROID_LOG_VERBOSE,
                            "AppName", "DEBUG: [%s] Code %i : %s",
                            pLayerPrefix, msgCode, pMsg);
    }

    // Returning false tells the layer not to stop when the event occurs, so
    // they see the same behavior with and without validation layers enabled.
    return VK_FALSE;
}

VKDevice::VKDevice(ANativeWindow *window)
{
    nativeWindow_ = window;

    CreateInstance();
    CreateSurface();
    CreatePhysicDevice();
    CreateDevice();
    queueFamilyIndices_ = FindQueueFamilies();
    CreateSwapchain();
    CreateQueue();
    CreateRenderPass();
    CreateFramebuffers();
    CreateCommandPool();
    CreateDescriptorPool();
    CreateCommandBuffers();
    SetupSynchronizeObjects();
}

VKDevice::~VKDevice()
{
    // TODO free vulkan resource

    if (vkDestroyDebugReportCallbackEXT) {
        vkDestroyDebugReportCallbackEXT(vkInstance_, vkDebugReportCallback_, nullptr);
    }
}

Buffer* VKDevice::CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data)
{
    Buffer* buffer = new VKBuffer(this, usageFlagBits, sharingMode, sizeOfBytes, data);
    RHI_SAFE_RETAIN(buffer);
    return buffer;
}

void VKDevice::WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size)
{
    ((VKBuffer*)buffer)->UpdateBuffer(data, offset, size);
}

// private

void VKDevice::CreateInstance()
{
    // Get layer count using null pointer as last parameter
    uint32_t instance_layer_present_count = 0;
    vkEnumerateInstanceLayerProperties(&instance_layer_present_count, nullptr);

    // Enumerate layers with valid pointer in last parameter
    VkLayerProperties* layer_props = (VkLayerProperties*)malloc(instance_layer_present_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&instance_layer_present_count, layer_props);

    // Make sure the desired validation layer is available
    const char *instance_layers[] = {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_GOOGLE_unique_objects"
    };

    uint32_t instance_layer_request_count = sizeof(instance_layers) / sizeof(instance_layers[0]);
    for (uint32_t i = 0; i < instance_layer_request_count; i++) {
        bool found = false;
        for (uint32_t j = 0; j < instance_layer_present_count; j++) {
            if (strcmp(instance_layers[i], layer_props[j].layerName) == 0) {
                found = true;
            }
        }
        if (!found) {
            assert(false);
        }
    }

    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .apiVersion = VK_MAKE_VERSION(1, 0, 0),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .pApplicationName = "Realxie's vulkan demo",
            .pEngineName = "RXEngine"
    };

    // prepare necessary extensions: Vulkan on Android need these to function
    std::vector<const char *> instanceExt;
    instanceExt.push_back("VK_KHR_surface");
    instanceExt.push_back("VK_KHR_android_surface");
    const char* DEBUG_REPORT_EXTENSION = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;

    // Enable Vulkan debug callback.
    {
        // Get the instance extension count
        uint32_t inst_ext_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &inst_ext_count, nullptr);

        // Enumerate the instance extensions
        VkExtensionProperties* inst_exts =
                (VkExtensionProperties *)malloc(inst_ext_count * sizeof(VkExtensionProperties));
        vkEnumerateInstanceExtensionProperties(nullptr, &inst_ext_count, inst_exts);

        const char * enabled_inst_exts[16] = {};
        uint32_t enabled_inst_ext_count = 0;

        // Make sure the debug report extension is available
        for (uint32_t i = 0; i < inst_ext_count; i++) {
            if (strcmp(inst_exts[i].extensionName,
                       VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0) {
                enabled_inst_exts[enabled_inst_ext_count++] =
                        VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
            }
        }

        if (enabled_inst_ext_count > 0)
        {
            instanceExt.push_back(DEBUG_REPORT_EXTENSION);
        }
    }

    // Create the Vulkan instance
    VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<uint32_t>(instanceExt.size()),
            .ppEnabledExtensionNames = instanceExt.data(),
            .enabledLayerCount = instance_layer_request_count,
            .ppEnabledLayerNames = instance_layers
    };

    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance_));

    if (!vkCreateDebugReportCallbackEXT)
    {
        vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)
                vkGetInstanceProcAddr(vkInstance_, "vkCreateDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)
                vkGetInstanceProcAddr(vkInstance_, "vkDestroyDebugReportCallbackEXT");
    }

    // Create the debug callback with desired settings
    if (vkCreateDebugReportCallbackEXT) {

        VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;
        debugReportCallbackCreateInfo.sType =
                VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        debugReportCallbackCreateInfo.pNext = NULL;
        debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                              VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                              VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        debugReportCallbackCreateInfo.pfnCallback = DebugReportCallback;
        debugReportCallbackCreateInfo.pUserData = NULL;

        CALL_VK(vkCreateDebugReportCallbackEXT(vkInstance_, &debugReportCallbackCreateInfo,
                                       nullptr, &vkDebugReportCallback_));
    }


}

void VKDevice::CreateSurface()
{
    RHI_ASSERT(nativeWindow_ != nullptr);
    RHI_ASSERT(vkInstance_ != nullptr);

    // if we create a surface, we need the surface extension
    VkAndroidSurfaceCreateInfoKHR createInfo{
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .window = nativeWindow_
    };
    vkCreateAndroidSurfaceKHR(vkInstance_, &createInfo, nullptr, &vkSurface_);
}

void VKDevice::CreatePhysicDevice()
{
    RHI_ASSERT(vkInstance_ != nullptr);

    uint32_t gpuCount = 0;
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, nullptr));
    VkPhysicalDevice tmpGpus[10] = {nullptr};
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, tmpGpus));

    // Pick up the first GPU Devicereturn true;
    vkPhysicalDevice_ = tmpGpus[0];
}

void VKDevice::CreateDevice()
{
    RHI_ASSERT(vkInstance_ != nullptr);
    RHI_ASSERT(vkPhysicalDevice_ != nullptr);
    RHI_ASSERT(vkPhysicalDevice_ != nullptr);

    // check for vulkan info on this GPU device
    VkPhysicalDeviceProperties gpuProperties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice_, &gpuProperties);
    LOGI("Vulkan Physical Device Name: %s", gpuProperties.deviceName);
    LOGI("Vulkan Physical Device Info: apiVersion: %x \n\t driverVersion: %x",
         gpuProperties.apiVersion, gpuProperties.driverVersion);
    LOGI("API Version Supported: %d.%d.%d", VK_VERSION_MAJOR(gpuProperties.apiVersion),
         VK_VERSION_MINOR(gpuProperties.apiVersion), VK_VERSION_PATCH(gpuProperties.apiVersion));

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice_, vkSurface_, &surfaceCapabilities);

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
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_, &queueFamilyCount, nullptr);
    RHI_ASSERT(queueFamilyCount);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_, &queueFamilyCount,
                                             queueFamilyProperties.data());

    uint32_t queueFamilyIndex;
    for (queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }
    RHI_ASSERT(queueFamilyIndex < queueFamilyCount);
    graphicQueueFamilyIndex_ = queueFamilyIndex;

    float priorities[] = {1.0f,};
    VkDeviceQueueCreateInfo queueCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .pNext = nullptr, .flags = 0, .queueCount = 1, .queueFamilyIndex = queueFamilyIndex, .pQueuePriorities = priorities,};

    // prepare necessary extensions: Vulkan on Android need these to function
    std::vector<const char *> deviceExt;
    deviceExt.push_back("VK_KHR_swapchain");

    VkDeviceCreateInfo deviceCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .pNext = nullptr, .queueCreateInfoCount = 1, .pQueueCreateInfos = &queueCreateInfo, .enabledLayerCount = 0, .ppEnabledLayerNames = nullptr, .enabledExtensionCount = static_cast<uint32_t>(deviceExt.size()), .ppEnabledExtensionNames = deviceExt.data(), .pEnabledFeatures = nullptr,};

    CALL_VK(vkCreateDevice(vkPhysicalDevice_, &deviceCreateInfo, nullptr, &vkDevice_));

    RHI_ASSERT(vkDevice_ != nullptr);
}

void VKDevice::CreateSwapchain()
{
    uint32_t queueFamilyIndices[] = {(uint32_t)queueFamilyIndices_.graphicsFamily,
                                     (uint32_t)queueFamilyIndices_.presentFamily};

    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkPhysicalDevice_, vkSurface_);
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vkSwapchainImageFormat_ = surfaceFormat.format;
    vkSwapchainExtent_ = ChooseSwapExtent(swapChainSupport.capabilities);
    viewport_ = {
            .x = 0,
            .y = 0,
            .width = (int)vkSwapchainExtent_.width,
            .height = (int)vkSwapchainExtent_.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
    };

    uint32_t minImageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
            minImageCount > swapChainSupport.capabilities.maxImageCount)
    {
        minImageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = vkSurface_,
        .minImageCount = minImageCount,
        .imageFormat = vkSwapchainImageFormat_,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = vkSwapchainExtent_,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
    };


    if (queueFamilyIndices_.graphicsFamily != queueFamilyIndices_.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional   createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    // create swapchain.
    CALL_VK(vkCreateSwapchainKHR(vkDevice_, &createInfo, nullptr, &vkSwapchain_));

    // setup swapChainImages
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(vkDevice_, vkSwapchain_, &imageCount, nullptr);
    swapChainImages_.resize(imageCount);
    vkGetSwapchainImagesKHR(vkDevice_, vkSwapchain_, &imageCount, swapChainImages_.data());

    // setup swapChainImageViews_
    swapChainImageViews_.resize(swapChainImages_.size());
    for (size_t i = 0; i < swapChainImages_.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages_[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = vkSwapchainImageFormat_;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        CALL_VK(vkCreateImageView(vkDevice_, &createInfo, nullptr, &swapChainImageViews_[i]));
    }
}


QueueFamilyIndices VKDevice::FindQueueFamilies()
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice_, i, vkSurface_, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }
        i++;
    }

    return indices;
}

VkSurfaceFormatKHR VKDevice::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
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

VkPresentModeKHR VKDevice::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
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

VkExtent2D VKDevice::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
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

SwapChainSupportDetails VKDevice::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
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

bool VKDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
}

uint32_t VKDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

void VKDevice::CreateRenderPass()
{
    VkAttachmentDescription attachmentDescriptions = {
            .format = vkSwapchainImageFormat_,
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
            .flags = 0,
            .inputAttachmentCount = 0,
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

    CALL_VK(vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_));
}

void VKDevice::CreateFramebuffers()
{
    RHI_ASSERT(swapChainImageViews_.size() > 0);
    RHI_ASSERT(vkSwapchainExtent_.width > 0 && vkSwapchainExtent_.height > 0);

    framebuffers_.resize(swapChainImageViews_.size());

    for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews_[i]};
        VkFramebufferCreateInfo framebufferInfo {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .renderPass = vkRenderPass_,
                .layers = 1,
                .attachmentCount = 1,  // 2 if using depth
                .pAttachments = attachments,
                .width = static_cast<uint32_t>(vkSwapchainExtent_.width),
                .height = static_cast<uint32_t>(vkSwapchainExtent_.height)
        };

        CALL_VK(vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &framebuffers_[i]));
    }
};

void VKDevice::CreateQueue()
{
    vkGetDeviceQueue(vkDevice_, queueFamilyIndices_.presentFamily, 0, &vkQueue_);
}

void VKDevice::CreateCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = 0
    };

    CALL_VK(vkCreateCommandPool(vkDevice_, &poolInfo, nullptr, &vkCommandPool_));
}

void VKDevice::CreateDescriptorPool()
{
    RHI_ASSERT(swapChainImages_.size() > 0);

    std::array<VkDescriptorPoolSize, 2> poolSizes = {
            VkDescriptorPoolSize {
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1, //static_cast<uint32_t>(swapChainImages_.size()),
            },
            VkDescriptorPoolSize {
                    .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorCount = 1, //static_cast<uint32_t>(swapChainImages_.size()),
            },
    };

    VkDescriptorPoolCreateInfo poolInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .poolSizeCount = (std::uint32_t)poolSizes.size(),
            .pPoolSizes = poolSizes.data(),
            .maxSets = static_cast<uint32_t>(swapChainImages_.size()),
    };

    CALL_VK(vkCreateDescriptorPool(vkDevice_, &poolInfo, nullptr, &vkDescriptorPool_));
}

void VKDevice::CreateCommandBuffers()
{
    RHI_ASSERT(framebuffers_.size() > 0);

    commandBuffers_.resize(framebuffers_.size());

    for (size_t i = 0; i < commandBuffers_.size(); i++) {
        VkCommandBufferAllocateInfo cmdBufferCreateInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext = nullptr,
                .commandPool = vkCommandPool_,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1
        };

        CALL_VK(vkAllocateCommandBuffers(vkDevice_, &cmdBufferCreateInfo, &commandBuffers_[i]));
    }
}

GraphicPipeline* VKDevice::CreateGraphicPipeline(const GraphicPipelineCreateInfo& graphicPipelineCreateInfo)
{
    GraphicPipeline* pipeline = new VKGraphicPipeline(this, graphicPipelineCreateInfo);
    RHI_SAFE_RETAIN(pipeline);
    return pipeline;
}

Shader* VKDevice::CreateShader(const std::vector<std::uint8_t>& shaderSource)
{
    Shader* shader = new VKShader(this, shaderSource);
    RHI_SAFE_RETAIN(shader);
    return shader;
}

RenderPass* VKDevice::CreateRenderPass(const RenderPassCreateInfo& createInfo)
{
    RenderPass* renderPass = new VKRenderPass(this, createInfo);
    RHI_SAFE_RETAIN(renderPass);
    return renderPass;
}

Texture* VKDevice::CreateTexture(const ImageCreateInfo& imageCreateInfo)
{
    Texture* texture = new VKTexture(this, imageCreateInfo);
    RHI_SAFE_RETAIN(texture);
    return texture;
}

Sampler* VKDevice::CreateSampler()
{
    Sampler* sampler = new VKSampler(this);
    RHI_SAFE_RETAIN(sampler);
    return sampler;
}

TextureView* VKDevice::CreateTextureView(const Texture* texture)
{
    TextureView* textureView = new VKTextureView(this, (VKTexture*)texture);
    RHI_SAFE_RETAIN(textureView);
    return textureView;
}

BindGroupLayout* VKDevice::CreateBindGroupLayout(const DescriptorSetLayoutCreateInfo& layoutCreateInfo)
{
    BindGroupLayout* bindGroupLayout = new VKBindGroupLayout(this, layoutCreateInfo);
    RHI_SAFE_RETAIN(bindGroupLayout);
    return bindGroupLayout;
}

BindGroup* VKDevice::CreateBindGroup(const BindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindingResources)
{
    BindGroup* bindGroup = new VKBindGroup(this, (VKBindGroupLayout*)bindGroupLayout, bindingResources);
    RHI_SAFE_RETAIN(bindGroup);
    return bindGroup;
}

PipelineLayout* VKDevice::CreatePipelineLayout(const std::vector<BindGroupLayout*>& bindGroupLayouts)
{
    PipelineLayout* pipelineLayout = new VKPipelineLayout(this, bindGroupLayouts);
    RHI_SAFE_RETAIN(pipelineLayout);
    return pipelineLayout;
}

BindingResource* VKDevice::CreateBindingResourceBuffer(std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size)
{
    BindingResource* bindingResource = new VKBindingBuffer(bindingPoint, (VKBuffer*)buffer, offset, size);
    RHI_SAFE_RETAIN(bindingResource);
    return bindingResource;
}

BindingResource* VKDevice::CreateBindingResourceCombined(std::uint32_t bindingPoint, const TextureView* textureView, const Sampler* sampler)
{
    BindingResource* bindingResource = new VKBindingCombined(bindingPoint, (VKTextureView*)textureView, (VKSampler*)sampler);
    RHI_SAFE_RETAIN(bindingResource);
    return bindingResource;
}

void VKDevice::WriteBindGroup(const BindGroup* bindGroup)
{
    RHI_CAST(const VKBindGroup*, bindGroup)->WriteToGPU();
}

void VKDevice::SetBindGroupToGraphicPipeline(const BindGroup *bindGroup,
                                             const GraphicPipeline *graphicPipeline)
{
    auto vkGraphicPipeline = RHI_CAST(const VKGraphicPipeline*, graphicPipeline);
    RHI_CAST(const VKBindGroup*, bindGroup)->BindToCommandBuffer(commandBuffers_[imageIndex_], vkGraphicPipeline->GetPipelineLayout());
}

void VKDevice::SetupSynchronizeObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    CALL_VK(vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore_));
    CALL_VK(vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphore_));

    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };
    CALL_VK(vkCreateFence(vkDevice_, &fenceCreateInfo, nullptr, &vkFence_));
}

void VKDevice::SetImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldImageLayout,
                    VkImageLayout newImageLayout, VkPipelineStageFlags srcStages,
                    VkPipelineStageFlags destStages)
{
    VkImageMemoryBarrier imageMemoryBarrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = NULL,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = oldImageLayout,
            .newLayout = newImageLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
            }
    };

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

void VKDevice::BeginRenderpass()
{
    CALL_VK(vkAcquireNextImageKHR(vkDevice_, vkSwapchain_, std::numeric_limits<uint64_t>::max(),
                                  vkRenderFinishedSemaphore_, VK_NULL_HANDLE, &imageIndex_));

    VkCommandBufferBeginInfo cmdBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
    };
    CALL_VK(vkBeginCommandBuffer(commandBuffers_[imageIndex_], &cmdBufferBeginInfo));

    // transition the display image to color attachment layout
    SetImageLayout(commandBuffers_[imageIndex_],
                   swapChainImages_[imageIndex_],
                   VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                   VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

    VkClearValue clearVals{
            .color.float32[0] = 1.0f,
            .color.float32[1] = 1.0f,
            .color.float32[2] = 0.0f,
            .color.float32[3] = 1.0f
    };

    VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = vkRenderPass_,
            .framebuffer = framebuffers_[imageIndex_],
            .renderArea = {
                    .offset = {.x = 0, .y = 0 },
                    .extent = vkSwapchainExtent_
            },
            .clearValueCount = 1,
            .pClearValues = &clearVals
    };

    vkCmdBeginRenderPass(commandBuffers_[imageIndex_], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VKDevice::EndRenderpass()
{
    vkCmdEndRenderPass(commandBuffers_[imageIndex_]);

    CALL_VK(vkEndCommandBuffer(commandBuffers_[imageIndex_]));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = &vkImageAvailableSemaphore_;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers_[imageIndex_];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &vkRenderFinishedSemaphore_;

    CALL_VK(vkResetFences(vkDevice_, 1, &vkFence_));
    if (auto code = vkQueueSubmit(vkQueue_, 1, &submitInfo, vkFence_); code != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    CALL_VK(vkWaitForFences(vkDevice_, 1, &vkFence_, VK_TRUE, std::numeric_limits<uint64_t>::max()));

    LOGI("Drawing frames......");

    VkResult result;
    VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &vkSwapchain_,
            .pImageIndices = &imageIndex_,
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

void VKDevice::BindVertexBuffer(Buffer* buffer, std::uint32_t offset)
{
    VkBuffer vkBuffer = ((VKBuffer*)buffer)->GetNative();
    VkBuffer vertexBuffers[] = {vkBuffer};
    VkDeviceSize offsets[] = {offset};
    vkCmdBindVertexBuffers(commandBuffers_[imageIndex_], 0, 1, vertexBuffers, offsets);
}

void VKDevice::BindIndexBuffer(Buffer* buffer, std::uint32_t offset)
{
    VkBuffer vkBuffer = ((VKBuffer*)buffer)->GetNative();
    vkCmdBindIndexBuffer(commandBuffers_[imageIndex_], vkBuffer, offset, VK_INDEX_TYPE_UINT16);
}

void VKDevice::Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance)
{
    vkCmdDraw(commandBuffers_[imageIndex_], vertexCount, instanceCount, firstVertex, firstInstance);
}

void VKDevice::Draw(std::uint32_t vertexCount, std::uint32_t firstVertex)
{
    vkCmdDraw(commandBuffers_[imageIndex_], vertexCount, 1, firstVertex, 0);
}

void VKDevice::DrawIndxed(std::uint32_t indexCount, std::uint32_t firstIndex)
{
    vkCmdDrawIndexed(commandBuffers_[imageIndex_], indexCount, 1, firstIndex, 0, 0);
}

void VKDevice::BindGraphicPipeline(GraphicPipeline* graphicPipeline)
{
    VKGraphicPipeline* pipeline = (VKGraphicPipeline*)graphicPipeline;
    pipeline->Bind(commandBuffers_[imageIndex_]);
}


NS_RHI_END
