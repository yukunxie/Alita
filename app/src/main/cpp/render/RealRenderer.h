//
// Created by realxie on 2019/3/23.
//

#ifndef ALITA_ALITA_H
#define ALITA_ALITA_H

#include <vector>
#include <android/log.h>
//#include <android_native_app_glue.h>
#include "types/Types.h"
#include "aux/Aux.h"
#include "../drivers/vulkan/vulkan_wrapper.h"
#include "RHI/backend/Vulkan/VKDevice.h"
#include "RHI/include/GraphicPipleine.h"
#include "RHI/include/RenderPass.h"

class RealRenderer
{
public:
    virtual ~RealRenderer();

    static RealRenderer *getInstance();

    inline bool isReady()
    { return rhiDevice_ != nullptr; }

    bool initVulkanContext(ANativeWindow* app);

    void drawFrame();

    void testRotate();

protected:
    RealRenderer();

private:
    static RealRenderer *instance_;

    RHI::VKDevice*          rhiDevice_          = nullptr;
    RHI::BufferHnd          rhiVertexBuffer_    = nullptr;
    RHI::BufferHnd          rhiIndexBuffer_     = nullptr;
    RHI::BufferHnd          rhiUniformBuffer_   = nullptr;
    RHI::UniformBufferObjectHnd rhiUBO          = nullptr;
    RHI::GraphicPipelineHnd rhiGraphicPipeline_ = nullptr;
    RHI::ShaderHnd          rhiVertShader_      = nullptr;
    RHI::ShaderHnd          rhiFragShader_      = nullptr;
    RHI::RenderPassHnd      rhiRenderPass_      = nullptr;
};
#endif //ALITA_ALITA_H
