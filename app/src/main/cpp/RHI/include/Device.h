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
#include "Texture.h"
#include "TextureView.h"
#include "Sampler.h"
#include "BindGroup.h"
#include "BindGroupLayout.h"
#include "PipelineLayout.h"
#include "BindingResource.h"

#include <vector>

NS_RHI_BEGIN

class Device
{
public:
    virtual Buffer* CreateBuffer(BufferUsageFlagBits usageFlagBits
            , SharingMode sharingMode
            , std::uint32_t sizeOfBytes
            , const void* data) = 0;

    virtual void WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size) = 0;

    virtual GraphicPipeline* CreateGraphicPipeline(const GraphicPipelineCreateInfo& graphicPipelineCreateInfo) = 0;

    virtual Shader* CreateShader(const std::vector<std::uint8_t>& shaderSource) = 0;

    virtual RenderPass* CreateRenderPass(const RenderPassCreateInfo& createInfo) = 0;

    virtual Texture* CreateTexture(const ImageCreateInfo& imageCreateInfo) = 0;

    virtual Sampler* CreateSampler() = 0;

    virtual TextureView* CreateTextureView(const Texture* texture) = 0;

    virtual BindGroupLayout* CreateBindGroupLayout(const DescriptorSetLayoutCreateInfo& layoutCreateInfo) = 0;

    virtual BindGroup* CreateBindGroup(const BindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindResources) = 0;

    virtual PipelineLayout* CreatePipelineLayout(const std::vector<BindGroupLayout*>& bindGroupLayouts) = 0;

    virtual BindingResource* CreateBindingResourceBuffer(std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) = 0;

    virtual BindingResource* CreateBindingResourceCombined(std::uint32_t bindingPoint, const TextureView* textureView, const Sampler* sampler) = 0;

    virtual void WriteBindGroup(const BindGroup* bindGroup) = 0;

    virtual void SetBindGroupToGraphicPipeline(const BindGroup *bindGroup,
                                               const GraphicPipeline *graphicPipeline) = 0;

    virtual void BindVertexBuffer(Buffer* buffer, std::uint32_t offset) = 0;

    virtual void BindIndexBuffer(Buffer* buffer, std::uint32_t offset) = 0;

    virtual void Draw(std::uint32_t vertexCount
            , std::uint32_t instanceCount
            , std::uint32_t firstVertex
            , std::uint32_t firstInstance) = 0;

    virtual void Draw(std::uint32_t vertexCount, std::uint32_t firstVertex) = 0;

    virtual void DrawIndxed(std::uint32_t indexCount, std::uint32_t firstIndex) = 0;

    virtual void BindGraphicPipeline(GraphicPipeline* graphicPipeline) = 0;

    virtual void BeginRenderpass() = 0;

    virtual void EndRenderpass() = 0;

    virtual Viewport GetViewport() = 0;

    virtual Scissor  GetScissor()  = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //ALITA_DEVICE_H
