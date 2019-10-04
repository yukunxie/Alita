//
// Created by realxie on 2019-10-02.
//

#include "VKDevice.h"
#include "VulkanMarcos.h"
#include "VKBuffer.h"
#include "VKGraphicPipeline.h"

#include <vector>
#include <array>

NS_RHI_BEGIN

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
    CreateCommandBuffers();
    SetupSynchronizeObjects();
}

VKDevice::~VKDevice()
{
    // TODO free vulkan resource
}

BufferHnd VKDevice::CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data)
{
    BufferHnd buffer(new VKBuffer(this, usageFlagBits, sharingMode, sizeOfBytes, data));
    return buffer;
//    // TODO define a class member
//    uint32_t queueFamilyIndex;
//
//    VkBufferCreateInfo bufferInfo = {
//            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//            .size = sizeOfBytes,
//            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//            .pQueueFamilyIndices = &queueFamilyIndex,
//            .queueFamilyIndexCount = 1,
//    };
//
//    VkBuffer buffer;
//    CALL_VK(vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &buffer));
//
//

    return nullptr;
}


// private

void VKDevice::CreateInstance()
{
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

    // Create the Vulkan instance
    VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<uint32_t>(instanceExt.size()),
            .ppEnabledExtensionNames = instanceExt.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr
    };

    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance_));
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

//        VkCommandBufferBeginInfo cmdBufferBeginInfo{
//                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
//                .pNext = nullptr,
//                .flags = 0,
//                .pInheritanceInfo = nullptr
//        };
//        CALL_VK(vkBeginCommandBuffer(commandBuffers_[i], &cmdBufferBeginInfo));
//
//        // transition the display image to color attachment layout
//        setImageLayout(commandBuffers_[i],
//                       swapChainImages_[i],
//                       VK_IMAGE_LAYOUT_UNDEFINED,
//                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
//                       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
//                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
//
//        // Now we start a renderpass. Any draw command has to be recorded in a
//        // renderpass
//        VkClearValue clearVals{
//                .color.float32[0] = 1.0f,
//                .color.float32[1] = 0.34f,
//                .color.float32[2] = 0.90f,
//                .color.float32[3] = 1.0f
//        };
//
//        VkRenderPassBeginInfo renderPassBeginInfo = {
//                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
//                .pNext = nullptr,
//                .renderPass = vkRenderPass_,
//                .framebuffer = swapChainFramebuffers_[i],
//                .renderArea = {.offset = {.x = 0, .y = 0 },.extent = swapChainExtent_},
//                .clearValueCount = 1,
//                .pClearValues = &clearVals
//        };
//
//        vkCmdBeginRenderPass(commandBuffers_[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//        vkCmdEndRenderPass(commandBuffers_[i]);
//
//        // transition back to swapchain image to PRESENT_SRC_KHR
//        CALL_VK(vkEndCommandBuffer(commandBuffers_[i]));
    }
}

GraphicPipelineHnd VKDevice::CreateGraphicPipeline(const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader)
{
    GraphicPipelineHnd pipeline(new VKGraphicPipeline(this, vertexShader, fragShader));
    return pipeline;

//    VkShaderModule vertShaderModule;
//    {
//        VkShaderModuleCreateInfo createInfo = {};
//        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//        createInfo.codeSize = vertexShader.size();
//        createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShader.data());
//
//        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &vertShaderModule) != VK_SUCCESS) {
//            throw std::runtime_error("failed to create shader module!");
//        }
//    }
//
//    VkShaderModule fragShaderModule;
//    {
//        VkShaderModuleCreateInfo createInfo = {};
//        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//        createInfo.codeSize = fragShader.size();
//        createInfo.pCode = reinterpret_cast<const uint32_t*>(fragShader.data());
//
//        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &fragShaderModule) != VK_SUCCESS) {
//            throw std::runtime_error("failed to create shader module!");
//        }
//    }
//
//    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//            .stage = VK_SHADER_STAGE_VERTEX_BIT,
//            .module = vertShaderModule,
//            .pName = "main",
//    };
//
//    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
//            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
//            .module = fragShaderModule,
//            .pName = "main",
//    };
//
//    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
//
//    VkVertexInputBindingDescription bindingDescription = {
//            .binding = 0,
//            .stride  = 5 * sizeof(float),
//            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
//    };
//
//    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {
//            VkVertexInputAttributeDescription {
//                    .binding = 0,
//                    .location = 0,
//                    .format = VK_FORMAT_R32G32_SFLOAT,
//                    .offset = 0
//            },
//            VkVertexInputAttributeDescription {
//                    .binding = 0,
//                    .location = 1,
//                    .format = VK_FORMAT_R32G32B32_SFLOAT,
//                    .offset = 8
//            }
//    };
//
//    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
//            .vertexBindingDescriptionCount = 1,
//            .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributeDescriptions.size()),
//            .pVertexBindingDescriptions = &bindingDescription,
//            .pVertexAttributeDescriptions = attributeDescriptions.data()
//    };
//
//    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
//            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
//            .primitiveRestartEnable = VK_FALSE
//    };
//
//    VkViewport viewport = {
//            .x = 0.0f,
//            .y = 0.0f,
//            .width = (float)vkSwapchainExtent_.width,
//            .height = (float)vkSwapchainExtent_.height,
//            .minDepth = 0.0f,
//            .maxDepth = 1.0f
//    };
//
//    VkRect2D scissor = {
//            .offset = {0, 0},
//            .extent = vkSwapchainExtent_,
//    };
//
//    VkPipelineViewportStateCreateInfo viewportState = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
//            .viewportCount = 1,
//            .pViewports = &viewport,
//            .scissorCount = 1,
//            .pScissors = &scissor,
//    };
//
//    VkPipelineRasterizationStateCreateInfo rasterizer = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
//            .depthClampEnable = VK_FALSE,
//            .rasterizerDiscardEnable = VK_FALSE,
//            .polygonMode = VK_POLYGON_MODE_FILL,
//            .lineWidth = 1.0f,
//            .cullMode = VK_CULL_MODE_NONE,
//            .depthBiasEnable = VK_FALSE,
//            .depthBiasConstantFactor = 0.0f,    // Optional
//            .depthBiasClamp = 0.0f,             // Optional
//            .depthBiasSlopeFactor = 0.0f,       // Optional
//    };
//
//    VkPipelineMultisampleStateCreateInfo multisampling = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
//            .sampleShadingEnable = VK_FALSE,
//            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
//            .minSampleShading = 1.0f, // Optional
//            .pSampleMask = nullptr, // Optional
//            .alphaToCoverageEnable = VK_FALSE, // Optional
//            .alphaToOneEnable = VK_FALSE, // Optional
//    };
//
//    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
//            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
//            .blendEnable = VK_FALSE,
//
//            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
//            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
//            .colorBlendOp = VK_BLEND_OP_ADD, // Optional
//            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
//            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
//            .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
//
//            //            .blendEnable = VK_TRUE,
//            //            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
//            //            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
//            //            .colorBlendOp = VK_BLEND_OP_ADD,
//            //            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
//            //            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
//            //            .alphaBlendOp = VK_BLEND_OP_ADD,
//    };
//
//    VkPipelineColorBlendStateCreateInfo colorBlending = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
//            .logicOpEnable = VK_FALSE,
//            .logicOp = VK_LOGIC_OP_COPY, // Optional
//            .attachmentCount = 1,
//            .pAttachments = &colorBlendAttachment,
//            .blendConstants[0] = 0.0f, // Optional
//            .blendConstants[1] = 0.0f, // Optional
//            .blendConstants[2] = 0.0f, // Optional
//            .blendConstants[3] = 0.0f, // Optional
//    };
//
//    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
//
//    VkPipelineDynamicStateCreateInfo dynamicState = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
//            .dynamicStateCount = 2,
//            .pDynamicStates = dynamicStates,
//    };
//
//    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
//            .setLayoutCount = 0, // Optional
//            .pSetLayouts = nullptr, // Optional
//            .pushConstantRangeCount = 0, // Optional
//            .pPushConstantRanges = nullptr, // Optional
//    };
//
//    if (vkCreatePipelineLayout(vkDevice_, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create pipeline layout!");
//    }
//
//    VkGraphicsPipelineCreateInfo pipelineInfo = {
//            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
//            .stageCount = 2,
//            .pStages = shaderStages,
//            .pVertexInputState = &vertexInputInfo,
//            .pInputAssemblyState = &inputAssembly,
//            .pViewportState = &viewportState,
//            .pRasterizationState = &rasterizer,
//            .pMultisampleState = &multisampling,
//            pipelineInfo.pDepthStencilState = nullptr, // Optional
//            .pColorBlendState = &colorBlending,
//            .pDynamicState = nullptr, // Optional
//
//            .layout = vkPipelineLayout_,
//            .renderPass = vkRenderPass_,
//            .subpass = 0,
//
//            .basePipelineHandle = VK_NULL_HANDLE, // Optional
//            .basePipelineIndex = -1, // Optional
//    };
//
//    if (vkCreateGraphicsPipelines(vkDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create graphics pipeline!"); }

    return nullptr;
}

//void RealRenderer::createVertexBuffer()
//{
//    const std::vector<TVertex> vertices = {{{0.0f, -.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
//    };
//
//    // TODO define a class member
//    uint32_t queueFamilyIndex_ = 0;
//
//    VkBufferCreateInfo bufferInfo = {
//            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
//            .size = sizeof(vertices[0]) * vertices.size(),
//            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//            .pQueueFamilyIndices = &queueFamilyIndex_,
//            .queueFamilyIndexCount = 1,
//    };
//
//    if (vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &vkVertexBuffer_) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create vertex buffer!");
//    }
//
//    // Allocate heap memory for vertex buffer
//
//    VkMemoryRequirements memRequirements;
//    vkGetBufferMemoryRequirements(vkDevice_, vkVertexBuffer_, &memRequirements);
//
//    std::uint32_t memTypeMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
//    VkMemoryAllocateInfo allocInfo = {
//            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
//            .allocationSize = memRequirements.size,
//            .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memTypeMask)
//    };
//
//    if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &vkVertexBufferMemory_) != VK_SUCCESS)
//    {
//        throw std::runtime_error("failed to allocate vertex buffer memory!");
//    }
//
//    void* data;
//    CALL_VK(vkMapMemory(vkDevice_, vkVertexBufferMemory_, 0, bufferInfo.size, 0, &data));
//    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
//    vkUnmapMemory(vkDevice_, vkVertexBufferMemory_);
//    CALL_VK(vkBindBufferMemory(vkDevice_, vkVertexBuffer_, vkVertexBufferMemory_, 0));
//}

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
                                     vkImageAvailableSemaphore_, VK_NULL_HANDLE, &imageIndex_));

    vkWaitForFences(vkDevice_, 1, &vkFence_, VK_TRUE, std::numeric_limits<uint64_t>::max());

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

//    vkCmdBindPipeline(commandBuffers_[imageIndex_], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);

//    // draw some commands
//    {
//        vkCmdBindPipeline(commandBuffers_[imageIndex_], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);
//        VkBuffer vertexBuffers[] = {vkVertexBuffer_};
//        VkDeviceSize offsets[] = {0};
//        vkCmdBindVertexBuffers(commandBuffers_[imageIndex_], 0, 1, vertexBuffers, offsets);
//
//        CALL_VK(vkCmdDraw(commandBuffers_[imageIndex_], 3, 1, 0, 0));
//    }
}

void VKDevice::EndRenderpass()
{
    vkCmdEndRenderPass(commandBuffers_[imageIndex_]);

    CALL_VK(vkEndCommandBuffer(commandBuffers_[imageIndex_]));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &vkImageAvailableSemaphore_;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers_[imageIndex_];

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

void VKDevice::BindBuffer(BufferHnd buffer, std::uint32_t offset)
{
    VkBuffer vkBuffer = ((VKBuffer*)buffer.get())->Get();
    VkBuffer vertexBuffers[] = {vkBuffer};
    VkDeviceSize offsets[] = {offset};
    vkCmdBindVertexBuffers(commandBuffers_[imageIndex_], 0, 1, vertexBuffers, offsets);
}

void VKDevice::Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance)
{
    vkCmdDraw(commandBuffers_[imageIndex_], vertexCount, instanceCount, firstVertex, firstInstance);
}

void VKDevice::Draw(std::uint32_t vertexCount, std::uint32_t firstVertex)
{
    vkCmdDraw(commandBuffers_[imageIndex_], 3, 1, 0, 0);
//    vkCmdDraw(commandBuffers_[imageIndex_], vertexCount, 1, firstVertex, 0);
}

void VKDevice::BindGraphicPipeline(GraphicPipelineHnd graphicPipeline)
{
    VKGraphicPipeline* pipeline = (VKGraphicPipeline*)graphicPipeline.get();
    pipeline->Bind(commandBuffers_[imageIndex_]);
}

NS_RHI_END
