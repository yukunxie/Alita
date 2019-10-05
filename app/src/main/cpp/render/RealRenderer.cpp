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

    rhiBuffer_ = rhiDevice_->CreateBuffer(RHI::BufferUsageFlagBits::VERTEX_BUFFER_BIT, RHI::SharingMode::EXCLUSIVE, sizeof(vertices[0]) * vertices.size(), vertices.data());

    RHI::RenderPassCreateInfo renderPassCreateInfo {
        .attachments = {
                RHI::AttachmentDescription {
                    .format = RHI::Format::B8G8R8_UNORM,
                    .samples = RHI::SampleCountFlagBits::SAMPLE_COUNT_1_BIT,
                    .loadOp = RHI::AttachmentLoadOp::CLEAR,
                    .storeOp = RHI::AttachmentStoreOp::STORE,
                    .stencilLoadOp = RHI::AttachmentLoadOp::DONT_CARE,
                    .stencilStoreOp = RHI::AttachmentStoreOp::DONT_CARE,
                    .initialLayout = RHI::ImageLayout::UNDEFINED,
                    .finalLayout = RHI::ImageLayout::PRESENT_SRC_KHR
                },
        },
        .subpasses = {
                RHI::SubpassDescription {
                    .colorAttachments = {
                            RHI::AttachmentReference {
                                .attachment = 0,
                                .layout = RHI::ImageLayout::COLOR_ATTACHMENT_OPTIMAL
                            },
                    },
                    .colorAttachments = {},
                    .depthStencilAttachment = {},
                    .inputAttachments = {},
                    .pipelineBindPoint = RHI::PipelineBindPoint::GRAPHICS,
                    .preserveAttachments = {},
                    .resolveAttachments = {}
                },
        },
        .dependencies = {}
    };

    rhiRenderPass_ = rhiDevice_->CreateRenderPass(renderPassCreateInfo);

    // ------------ Start setup GraphicPipeline object ---------------

    // Step 1. create shaders

    const TData& vertData = AFileSystem::getInstance()->readData("shaders/shader.vert.spv");
    const TData& fragData = AFileSystem::getInstance()->readData("shaders/shader.frag.spv");
    rhiVertShader_ = rhiDevice_->CreateShader(vertData);
    rhiFragShader_ = rhiDevice_->CreateShader(fragData);

    std::vector<RHI::PipelineShaderStageCreateInfo> shaderStageInfos = {
            RHI::PipelineShaderStageCreateInfo {
                .stage = RHI::ShaderStageFlagBits::VERTEX_BIT,
                .shader = rhiVertShader_.get(),
                .entryName = "main"
            },

            RHI::PipelineShaderStageCreateInfo {
                .stage = RHI::ShaderStageFlagBits::FRAGMENT_BIT,
                .shader = rhiFragShader_.get(),
                .entryName = "main"
            }
    };

    // Step 2. setup vertex attribute info
    
    RHI::PipelineVertexInputStateCreateInfo vertexInputInfo = {
            .vertexBindingDescriptions = {
                    RHI::VertexInputBindingDescription {
                            .binding = 0,
                            .stride  = sizeof(TVertex),
                            .inputRate = RHI::VertexInputRate::VERTEX
                    },
            },
            .vertexAttributeDescriptions = {
                    RHI::VertexInputAttributeDescription {
                            .binding = 0,
                            .location = 0,
                            .format = RHI::Format::R32G32_SFLOAT,
                            .offset = offsetof(TVertex, pos)
                    },
                    RHI::VertexInputAttributeDescription {
                            .binding = 0,
                            .location = 1,
                            .format = RHI::Format::R32G32B32_SFLOAT,
                            .offset = offsetof(TVertex, color)
                    },
            }
    };

    // Step 3. setup viewport and scissor

    RHI::Viewport viewport = rhiDevice_->GetViewport();
    std::vector<RHI::Viewport> viewports = {
            viewport,
    };

//      // Two viewports.
//    std::vector<RHI::Viewport> viewports = {
//            RHI::Viewport {
//                .x = 0, .y = 0, .width = viewport.width, .height = viewport.height / 2, .minDepth = 0.0f, .maxDepth = 1.0f
//            },
//            RHI::Viewport {
//                    .x = 0, .y = viewport.height / 2, .width = viewport.width, .height = viewport.height / 2, .minDepth = 0.0f, .maxDepth = 1.0f
//            },
//    };

    std::vector<RHI::Scissor>  scissors = {
            RHI::Scissor{
                .x = 0, .y = 0, .width = (std::uint32_t)viewport.width, .height = (std::uint32_t)viewport.height
            },
    };

    RHI::PipelineViewportStateCreateInfo viewportState = {
            .viewports = std::move(viewports),
            .scissors  = std::move(scissors),
    };

    // final, create graphic pipeline state.

    rhiGraphicPipeline_ = rhiDevice_->CreateGraphicPipeline(shaderStageInfos, vertexInputInfo, viewportState);

    // ------------ End setup GraphicPipeline object ---------------

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
