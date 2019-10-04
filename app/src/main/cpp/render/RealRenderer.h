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
#include "RHI/backend/Vulkan/VKDevice.h"
#include "RHI/include/GraphicPipleine.h"

class RealRenderer
{
public:
    virtual ~RealRenderer();

    static RealRenderer *getInstance();

    inline bool isReady()
    { return rhiDevice_ != nullptr; }

    bool initVulkanContext(ANativeWindow* app);

    void drawFrame();

protected:
    RealRenderer();

private:
    static RealRenderer *instance_;

    RHI::VKDevice*          rhiDevice_          = nullptr;
    RHI::BufferHnd          rhiBuffer_          = nullptr;
    RHI::GraphicPipelineHnd rhiGraphicPipeline_ = nullptr;
};
#endif //VULKANDEMO_VULKANDEMO_H
