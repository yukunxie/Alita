//
// Created by realxie on 2019/3/23.
//

#include "RealRenderer.h"

#include <vector>
#include <array>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../drivers/vulkan/vulkan_wrapper.h"
#include "../aux/AFileSystem.h"
#include "../external/glm/vec3.hpp"
#include "../external/glm/gtx/closest_point.inl"
#include "../aux/AFileSystem.h"
//
//// Android log function wrappers
//static const char *kTAG = "VulkanDemo";
//#define LOGI(...) \
//  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
//#define LOGW(...) \
//  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
//#define LOGE(...) \
//  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))
//
// Vulkan call wrapper
//#define CALL_VK(func)                                                 \
//  if (VK_SUCCESS != (func)) {                                         \
//    __android_log_print(ANDROID_LOG_ERROR, "Tutorial ",               \
//                        "Vulkan error. File[%s], line[%d]", __FILE__, \
//                        __LINE__);                                    \
//  }

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

bool RealRenderer::initVulkanContext(ANativeWindow *window)
{
    if (window == nullptr || isReady()) {
        return false;
    }

    rhiDevice_ = new RHI::VKDevice(window);

    const std::vector<TVertex> vertices = {{{0.0f, -.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    rhiBuffer_ = rhiDevice_->CreateBuffer(RHI::BufferUsageFlagBits::BUFFER_USAGE_VERTEX_BUFFER_BIT, RHI::SharingMode::SHARING_MODE_EXCLUSIVE, sizeof(vertices[0]) * vertices.size(), vertices.data());

    const TData& vertData = AFileSystem::getInstance()->readData("shaders/shader.vert.spv");
    const TData& fragData = AFileSystem::getInstance()->readData("shaders/shader.frag.spv");

    rhiGraphicPipeline_ = rhiDevice_->CreateGraphicPipeline(vertData, fragData);

    return true;
}

void RealRenderer::drawFrame()
{
    rhiDevice_->BeginRenderpass();
    rhiDevice_->BindGraphicPipeline(rhiGraphicPipeline_);
    rhiDevice_->BindBuffer(rhiBuffer_, 0);
    rhiDevice_->Draw(3, 0);
    rhiDevice_->EndRenderpass();
}
