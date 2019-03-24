//
// Created by realxie on 2019/3/23.
//

#include "VulkanDemo.h"

#include <vector>

#include "vulkan_wrapper.h"

// Android log function wrappers
static const char* kTAG = "Vulkan-Tutorial01";
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
    assert(false);                                                    \
  }


//VkInstance tutorialInstance;
//VkPhysicalDevice tutorialGpu;
//VkDevice tutorialDevice;
//VkSurfaceKHR tutorialSurface;
//bool         isInited = false;

bool initVulkanInstance(struct android_app* app)
{
//    if (!InitVulkan() || isInited) {
//        return false;
//    }
//
//    VkApplicationInfo appInfo = {
//            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
//            .pNext = nullptr,
//            .apiVersion = VK_MAKE_VERSION(1, 0, 0),
//            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
//            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
//            .pApplicationName = "Realxie's vulkan demo",
//            .pEngineName = "demo",
//    };
//
//    // prepare necessary extensions: Vulkan on Android need these to function
//    std::vector<const char *> instanceExt, deviceExt;
//    instanceExt.push_back("VK_KHR_surface");
//    instanceExt.push_back("VK_KHR_android_surface");
//    deviceExt.push_back("VK_KHR_swapchain");
//
//    // Create the Vulkan instance
//    VkInstanceCreateInfo instanceCreateInfo{
//            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
//            .pNext = nullptr,
//            .pApplicationInfo = &appInfo,
//            .enabledExtensionCount = static_cast<uint32_t>(instanceExt.size()),
//            .ppEnabledExtensionNames = instanceExt.data(),
//            .enabledLayerCount = 0,
//            .ppEnabledLayerNames = nullptr,
//    };
//
//    VkResult result;
//    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &tutorialInstance));
//
//    // if we create a surface, we need the surface extension
//    VkAndroidSurfaceCreateInfoKHR createInfo{
//            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
//            .pNext = nullptr,
//            .flags = 0,
//            .window = app->window};
//    CALL_VK(vkCreateAndroidSurfaceKHR(tutorialInstance, &createInfo, nullptr, &tutorialSurface));
//
//    uint32_t gpuCount = 0;
//    CALL_VK(vkEnumeratePhysicalDevices(tutorialInstance, &gpuCount, nullptr));
//    VkPhysicalDevice tmpGpus[gpuCount];
//    CALL_VK(vkEnumeratePhysicalDevices(tutorialInstance, &gpuCount, tmpGpus));
//    tutorialGpu = tmpGpus[0];  // Pick up the first GPU Devicereturn true;
//
//    // check for vulkan info on this GPU device
//    VkPhysicalDeviceProperties gpuProperties;
//    vkGetPhysicalDeviceProperties(tutorialGpu, &gpuProperties);
//    LOGI("Vulkan Physical Device Name: %s", gpuProperties.deviceName);
//    LOGI("Vulkan Physical Device Info: apiVersion: %x \n\t driverVersion: %x",
//         gpuProperties.apiVersion, gpuProperties.driverVersion);
//    LOGI("API Version Supported: %d.%d.%d",
//         VK_VERSION_MAJOR(gpuProperties.apiVersion),
//         VK_VERSION_MINOR(gpuProperties.apiVersion),
//         VK_VERSION_PATCH(gpuProperties.apiVersion));
//
//    VkSurfaceCapabilitiesKHR surfaceCapabilities;
//    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(tutorialGpu, tutorialSurface,
//                                              &surfaceCapabilities);
//
//    LOGI("Vulkan Surface Capabilities:\n");
//    LOGI("\timage count: %u - %u\n", surfaceCapabilities.minImageCount,
//         surfaceCapabilities.maxImageCount);
//    LOGI("\tarray layers: %u\n", surfaceCapabilities.maxImageArrayLayers);
//    LOGI("\timage size (now): %dx%d\n", surfaceCapabilities.currentExtent.width,
//         surfaceCapabilities.currentExtent.height);
//    LOGI("\timage size (extent): %dx%d - %dx%d\n",
//         surfaceCapabilities.minImageExtent.width,
//         surfaceCapabilities.minImageExtent.height,
//         surfaceCapabilities.maxImageExtent.width,
//         surfaceCapabilities.maxImageExtent.height);
//    LOGI("\tusage: %x\n", surfaceCapabilities.supportedUsageFlags);
//    LOGI("\tcurrent transform: %u\n", surfaceCapabilities.currentTransform);
//    LOGI("\tallowed transforms: %x\n", surfaceCapabilities.supportedTransforms);
//    LOGI("\tcomposite alpha flags: %u\n", surfaceCapabilities.currentTransform);
//
//    // Find a GFX queue family
//    uint32_t queueFamilyCount;
//    vkGetPhysicalDeviceQueueFamilyProperties(tutorialGpu, &queueFamilyCount, nullptr);
//    assert(queueFamilyCount);
//    std::vector<VkQueueFamilyProperties>  queueFamilyProperties(queueFamilyCount);
//    vkGetPhysicalDeviceQueueFamilyProperties(tutorialGpu, &queueFamilyCount,
//                                             queueFamilyProperties.data());
//
//    uint32_t queueFamilyIndex;
//    for (queueFamilyIndex=0; queueFamilyIndex < queueFamilyCount;
//         queueFamilyIndex++) {
//        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//            break;
//        }
//    }
//    assert(queueFamilyIndex < queueFamilyCount);
//
//    // Create a logical device from GPU we picked
//    float priorities[] = {
//            1.0f,
//    };
//    VkDeviceQueueCreateInfo queueCreateInfo{
//            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
//            .pNext = nullptr,
//            .flags = 0,
//            .queueCount = 1,
//            .queueFamilyIndex = queueFamilyIndex,
//            .pQueuePriorities = priorities,
//    };
//
//    VkDeviceCreateInfo deviceCreateInfo{
//            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
//            .pNext = nullptr,
//            .queueCreateInfoCount = 1,
//            .pQueueCreateInfos = &queueCreateInfo,
//            .enabledLayerCount = 0,
//            .ppEnabledLayerNames = nullptr,
//            .enabledExtensionCount = static_cast<uint32_t>(deviceExt.size()),
//            .ppEnabledExtensionNames = deviceExt.data(),
//            .pEnabledFeatures = nullptr,
//    };
//
//    CALL_VK(
//            vkCreateDevice(tutorialGpu, &deviceCreateInfo, nullptr, &tutorialDevice));

            return true;
}

RealRenderer* RealRenderer::instance_ = nullptr;

RealRenderer* RealRenderer::getInstance()
{
    if (instance_)
    {
        return instance_;
    }
    instance_ =  new RealRenderer();
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

bool RealRenderer::initVulkanContext(struct android_app* app)
{
    if (app->window == nullptr || isReady())
    {
        return false;
    }

    vkInstance_ = createVKInstance(app);
    vkSurface_  = createVKSurface(app);
    vkGPU_      = createVKGPU(app);
    vkDevice_   = createVKDevice(app);

    return true;
}

VkInstance RealRenderer::createVKInstance(struct android_app* app)
{
    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .apiVersion = VK_MAKE_VERSION(1, 0, 0),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .pApplicationName = "Realxie's vulkan demo",
            .pEngineName = "demo",
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
            .ppEnabledLayerNames = nullptr,
    };

    VkInstance instance;
    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));

    return instance;
}

VkPhysicalDevice RealRenderer::createVKGPU(struct android_app* app)
{
    uint32_t gpuCount = 0;
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, nullptr));
    VkPhysicalDevice tmpGpus[gpuCount];
    CALL_VK(vkEnumeratePhysicalDevices(vkInstance_, &gpuCount, tmpGpus));
    return tmpGpus[0];  // Pick up the first GPU Devicereturn true;
}

VkDevice RealRenderer::createVKDevice(struct android_app* app)
{
    // check for vulkan info on this GPU device
    VkPhysicalDeviceProperties gpuProperties;
    vkGetPhysicalDeviceProperties(vkGPU_, &gpuProperties);
    LOGI("Vulkan Physical Device Name: %s", gpuProperties.deviceName);
    LOGI("Vulkan Physical Device Info: apiVersion: %x \n\t driverVersion: %x",
         gpuProperties.apiVersion, gpuProperties.driverVersion);
    LOGI("API Version Supported: %d.%d.%d",
         VK_VERSION_MAJOR(gpuProperties.apiVersion),
         VK_VERSION_MINOR(gpuProperties.apiVersion),
         VK_VERSION_PATCH(gpuProperties.apiVersion));

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkGPU_, vkSurface_,
                                              &surfaceCapabilities);

    LOGI("Vulkan Surface Capabilities:\n");
    LOGI("\timage count: %u - %u\n", surfaceCapabilities.minImageCount,
         surfaceCapabilities.maxImageCount);
    LOGI("\tarray layers: %u\n", surfaceCapabilities.maxImageArrayLayers);
    LOGI("\timage size (now): %dx%d\n", surfaceCapabilities.currentExtent.width,
         surfaceCapabilities.currentExtent.height);
    LOGI("\timage size (extent): %dx%d - %dx%d\n",
         surfaceCapabilities.minImageExtent.width,
         surfaceCapabilities.minImageExtent.height,
         surfaceCapabilities.maxImageExtent.width,
         surfaceCapabilities.maxImageExtent.height);
    LOGI("\tusage: %x\n", surfaceCapabilities.supportedUsageFlags);
    LOGI("\tcurrent transform: %u\n", surfaceCapabilities.currentTransform);
    LOGI("\tallowed transforms: %x\n", surfaceCapabilities.supportedTransforms);
    LOGI("\tcomposite alpha flags: %u\n", surfaceCapabilities.currentTransform);

    // Find a GFX queue family
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(vkGPU_, &queueFamilyCount, nullptr);
    assert(queueFamilyCount);
    std::vector<VkQueueFamilyProperties>  queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkGPU_, &queueFamilyCount,
                                             queueFamilyProperties.data());

    uint32_t queueFamilyIndex;
    for (queueFamilyIndex=0; queueFamilyIndex < queueFamilyCount;
         queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }
    assert(queueFamilyIndex < queueFamilyCount);

    float priorities[] = {
            1.0f,
    };
    VkDeviceQueueCreateInfo queueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCount = 1,
            .queueFamilyIndex = queueFamilyIndex,
            .pQueuePriorities = priorities,
    };

    // prepare necessary extensions: Vulkan on Android need these to function
    std::vector<const char *> deviceExt;
    deviceExt.push_back("VK_KHR_swapchain");

    VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<uint32_t>(deviceExt.size()),
            .ppEnabledExtensionNames = deviceExt.data(),
            .pEnabledFeatures = nullptr,
    };

    VkDevice device;
    CALL_VK(vkCreateDevice(vkGPU_, &deviceCreateInfo, nullptr, &device));

    return device;
}

VkSurfaceKHR  RealRenderer::createVKSurface(struct android_app* app)
{
    // if we create a surface, we need the surface extension
    VkAndroidSurfaceCreateInfoKHR createInfo{
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .window = app->window};
    VkSurfaceKHR surface;
    CALL_VK(vkCreateAndroidSurfaceKHR(vkInstance_, &createInfo, nullptr, &surface));
    return surface;
}

