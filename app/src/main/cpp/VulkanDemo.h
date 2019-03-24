//
// Created by realxie on 2019/3/23.
//

#ifndef VULKANDEMO_VULKANDEMO_H
#define VULKANDEMO_VULKANDEMO_H

#include <android/log.h>
#include <android_native_app_glue.h>

#include "vulkan_wrapper.h"

class RealRenderer
{
public:
    virtual ~RealRenderer();
    static RealRenderer* getInstance();

    inline bool isReady(){return vkDevice_ != nullptr;}
    bool initVulkanContext(struct android_app* app);

protected:
    RealRenderer();

private:
    VkInstance          createVKInstance(struct android_app* app);
    VkPhysicalDevice    createVKGPU(struct android_app* app);
    VkDevice            createVKDevice(struct android_app* app);
    VkSurfaceKHR        createVKSurface(struct android_app* app);

private:
    VkInstance          vkInstance_ = nullptr;
    VkPhysicalDevice    vkGPU_      = nullptr;
    VkDevice            vkDevice_   = nullptr;
    VkSurfaceKHR        vkSurface_;

    static RealRenderer* instance_;
};

bool initVulkanInstance(struct android_app* app);

#endif //VULKANDEMO_VULKANDEMO_H
