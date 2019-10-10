//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_DEVICE_H
#define ALITA_DEVICE_H

#include "Macros.h"
#include "Buffer.h"
#include "GraphicPipleine.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Flags.h"
#include "UniformBufferObject.h"
#include "Texture.h"
#include "Sampler.h"

#include <vector>

NS_RHI_BEGIN

class Device
{
public:
    virtual BufferHnd CreateBuffer(BufferUsageFlagBits usageFlagBits
            , SharingMode sharingMode
            , std::uint32_t sizeOfBytes
            , const void* data) = 0;

    virtual void WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size) = 0;

    virtual GraphicPipelineHnd CreateGraphicPipeline(const std::vector<RHI::PipelineShaderStageCreateInfo>& shaderStageInfos
            , const PipelineVertexInputStateCreateInfo& vertexInputInfo
            , const PipelineViewportStateCreateInfo& viewportState) = 0;

    virtual ShaderHnd CreateShader(const std::vector<std::uint8_t>& shaderSource) = 0;

    virtual RenderPassHnd CreateRenderPass(const RenderPassCreateInfo& createInfo) = 0;

    virtual UniformBufferObjectHnd CreateUniformBufferObject(const GraphicPipeline* graphicPipeline) = 0;

    virtual UniformBufferObjectHnd CreateUniformBufferObject(const GraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) = 0;

    virtual UniformBufferObjectHnd CreateUniformBufferObject(const GraphicPipeline* graphicPipeline, std::uint32_t bindingPoint, const Texture* texture, const Sampler* sampler) = 0;

    virtual TextureHnd CreateTexture(const ImageCreateInfo& imageCreateInfo) = 0;

    virtual SamplerHnd CreateSampler() = 0;

    virtual void BindVertexBuffer(BufferHnd buffer, std::uint32_t offset) = 0;

    virtual void BindIndexBuffer(BufferHnd buffer, std::uint32_t offset) = 0;

    virtual void BindUniformBufferObject(const UniformBufferObject* ubo, const GraphicPipeline* graphicPipeline, std::uint32_t bindingPoint) = 0;

    virtual void Draw(std::uint32_t vertexCount
            , std::uint32_t instanceCount
            , std::uint32_t firstVertex
            , std::uint32_t firstInstance) = 0;

    virtual void Draw(std::uint32_t vertexCount, std::uint32_t firstVertex) = 0;

    virtual void DrawIndxed(std::uint32_t indexCount, std::uint32_t firstIndex) = 0;

    virtual void BindGraphicPipeline(GraphicPipelineHnd graphicPipeline) = 0;

    virtual void BeginRenderpass() = 0;

    virtual void EndRenderpass() = 0;

    virtual Viewport GetViewport() = 0;

    virtual void UpdateUniformBufferObject(UniformBufferObject* ubo, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) = 0;

    virtual Scissor  GetScissor()  = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //ALITA_DEVICE_H
