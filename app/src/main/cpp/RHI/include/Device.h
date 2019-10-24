//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_DEVICE_H
#define ALITA_DEVICE_H

#include "BindGroup.h"
#include "BindGroupLayout.h"
#include "BindingResource.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "CommandEncoder.h"
#include "Flags.h"
#include "RenderPipleine.h"
#include "Macros.h"
#include "PipelineLayout.h"
#include "Queue.h"
#include "RHI.h"
#include "RHIObjectBase.h"
#include "RenderPass.h"
#include "RenderPassEncoder.h"
#include "RenderQueue.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureView.h"
#include "SwapChain.h"

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

    virtual RenderPipeline* CreateRenderPipeline(const RenderPipelineDescriptor& descriptor) = 0;

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

    virtual Queue* CreateQueue() = 0;

    virtual CommandEncoder* CreateCommandEncoder() = 0;

    virtual SwapChain* CreateSwapChain() = 0;

    virtual void WriteBindGroup(const BindGroup* bindGroup) = 0;

    virtual Viewport GetViewport() = 0;

    virtual Scissor  GetScissor()  = 0;

    virtual Queue*   GetQueue() = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //ALITA_DEVICE_H
