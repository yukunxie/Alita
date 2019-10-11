//
// Created by realxie on 2019/3/23.
//

#include "RealRenderer.h"

#include <vector>
#include <array>
#include <chrono>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "../drivers/vulkan/vulkan_wrapper.h"
#include "../aux/AFileSystem.h"
#include "../external/glm/vec3.hpp"
#include "../external/glm/gtx/closest_point.inl"
#include "../aux/AFileSystem.h"

#include "RHI/include/RHI.h"

#include "external/stb/stb_image.h"

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

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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

    const std::vector<TVertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    rhiVertexBuffer_ = rhiDevice_->CreateBuffer(RHI::BufferUsageFlagBits::VERTEX_BUFFER_BIT, RHI::SharingMode::EXCLUSIVE, sizeof(vertices[0]) * vertices.size(), vertices.data());

    const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0};
    rhiIndexBuffer_ = rhiDevice_->CreateBuffer(RHI::BufferUsageFlagBits::INDEX_BUFFER_BIT, RHI::SharingMode::EXCLUSIVE, sizeof(indices[0]) * indices.size(), indices.data());

    UniformBufferObject ubo = {};
    float time = 0.0f;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), rhiDevice_->GetViewport().width / (float) rhiDevice_->GetViewport().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    rhiUniformBuffer_ = rhiDevice_->CreateBuffer(RHI::BufferUsageFlagBits::UNIFORM_BUFFER_BIT, RHI::SharingMode::EXCLUSIVE, sizeof(UniformBufferObject), &ubo);

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
                .shader = rhiVertShader_,
                .entryName = "main"
            },

            RHI::PipelineShaderStageCreateInfo {
                .stage = RHI::ShaderStageFlagBits::FRAGMENT_BIT,
                .shader = rhiFragShader_,
                .entryName = "main"
            }
    };

    //

    RHI::DescriptorSetLayoutCreateInfo layoutCreateInfo{
        .bindings = {
              RHI::DescriptorSetLayoutBinding {
                  .binding = 0,
                  .descriptorCount = 1,
                  .descriptorType = RHI::DescriptorType::UNIFORM_BUFFER,
                  .stageFlags = (std::uint32_t)RHI::ShaderStageFlagBits::VERTEX_BIT
              },
              RHI::DescriptorSetLayoutBinding {
                      .binding = 1,
                      .descriptorCount = 1,
                      .descriptorType = RHI::DescriptorType::COMBINED_IMAGE_SAMPLER,
                      .stageFlags = (std::uint32_t)RHI::ShaderStageFlagBits::FRAGMENT_BIT
              },
        }
    };
    rhiBindGroupLayout_ = rhiDevice_->CreateBindGroupLayout(layoutCreateInfo);

    rhiPipelineLayout_ = rhiDevice_->CreatePipelineLayout({rhiBindGroupLayout_});

//    rhiBindGroup_ = rhiDevice_->CreateBindGroup(rhiBindGroupLayout_, )

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
                    RHI::VertexInputAttributeDescription {
                            .binding = 0,
                            .location = 2,
                            .format = RHI::Format::R32G32_SFLOAT,
                            .offset = offsetof(TVertex, texCoord)
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

    // final, create graphic pipeline state.create

    RHI::GraphicPipelineCreateInfo graphicPipelineCreateInfo {
            .pPipelineLayout = rhiPipelineLayout_,
            .viewportState = viewportState,
            .shaderStageInfos = shaderStageInfos,
            .vertexInputInfo = vertexInputInfo,
    };

    rhiGraphicPipeline_ = rhiDevice_->CreateGraphicPipeline(graphicPipelineCreateInfo);

    // ------------ End setup GraphicPipeline object ---------------

    // setup image

    int texWidth, texHeight, texChannels;
    const TData& imageData = AFileSystem::getInstance()->readData("images/spiderman.jpg");
    stbi_uc* pixels = stbi_load_from_memory(imageData.data(), imageData.size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

//    ImageCreateFlags       flags;
//    ImageType              imageType;
//    Format                 format;
//    Extent3D               extent;
//    uint32_t               mipLevels;
//    uint32_t               arrayLayers;
//    SampleCountFlagBits    samples;
//    ImageTiling            tiling;
//    ImageUsageFlags        usage;
//    SharingMode            sharingMode;
//    ImageLayout            initialLayout;
//    const void*            imageData;

    RHI::ImageCreateInfo imageCreateInfo {
        .imageType = RHI::ImageType::IMAGE_TYPE_2D,
        .format    = RHI::Format::R8G8B8A8_UNORM,
        .extent = {
            .width = (std::uint32_t)texWidth,
            .height= (std::uint32_t)texHeight,
            .depth = 1
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = RHI::SampleCountFlagBits::SAMPLE_COUNT_1_BIT,
        .tiling = RHI::ImageTiling::LINEAR,
        .sharingMode = RHI::SharingMode::EXCLUSIVE,
        .imageData = pixels,
    };

    rhiTexture_ = rhiDevice_->CreateTexture(imageCreateInfo);
    rhiTextureView_ = rhiDevice_->CreateTextureView(rhiTexture_);

    stbi_image_free(pixels);

    rhiSampler_ = rhiDevice_->CreateSampler();

    // setup UBO
    rhiBindingBuffer_ = rhiDevice_->CreateBindingResourceBuffer(0, rhiUniformBuffer_, 0, sizeof(UniformBufferObject));
    rhiBindingCombined_ = rhiDevice_->CreateBindingResourceCombined(1, rhiTextureView_, rhiSampler_);

    rhiBindGroup_ = rhiDevice_->CreateBindGroup(rhiBindGroupLayout_, {rhiBindingBuffer_, rhiBindingCombined_});
    rhiDevice_->WriteBindGroup(rhiBindGroup_);

    return true;
}

void RealRenderer::testRotate()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo = {};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), rhiDevice_->GetViewport().width / (float) rhiDevice_->GetViewport().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    rhiDevice_->WriteBuffer(rhiUniformBuffer_, &ubo, 0, sizeof(UniformBufferObject));
}

void RealRenderer::drawFrame()
{
    testRotate();
    rhiDevice_->BeginRenderpass();
    rhiDevice_->BindGraphicPipeline(rhiGraphicPipeline_);
    rhiDevice_->BindVertexBuffer(rhiVertexBuffer_, 0);
    rhiDevice_->BindIndexBuffer(rhiIndexBuffer_, 0);
    rhiDevice_->SetBindGroupToGraphicPipeline(rhiBindGroup_, rhiGraphicPipeline_);
    rhiDevice_->DrawIndxed(6, 0);
//    rhiDevice_->Draw(3, 0);
    rhiDevice_->EndRenderpass();
}
