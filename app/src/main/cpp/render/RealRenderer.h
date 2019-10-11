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

#include "RHI/include/RHI.h"

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
    RHI::BindGroupHnd       rhiBindGroup_        = nullptr;
    RHI::BindingResourceHnd rhiBindingBuffer_    = nullptr;
    RHI::BindingResourceHnd rhiBindingCombined_  = nullptr;
    RHI::PipelineLayoutHnd  rhiPipelineLayout_  = nullptr;
    RHI::BindGroupLayoutHnd rhiBindGroupLayout_  = nullptr;
    RHI::TextureHnd         rhiTexture_         = nullptr;
    RHI::TextureViewHnd     rhiTextureView_     = nullptr;
    RHI::SamplerHnd         rhiSampler_         = nullptr;
    RHI::GraphicPipelineHnd rhiGraphicPipeline_ = nullptr;
    RHI::ShaderHnd          rhiVertShader_      = nullptr;
    RHI::ShaderHnd          rhiFragShader_      = nullptr;
    RHI::RenderPassHnd      rhiRenderPass_      = nullptr;
};
#endif //ALITA_ALITA_H
