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
    virtual Buffer* CreateBuffer(const BufferDescriptor& descriptor) = 0;

    virtual void WriteBuffer(const Buffer* buffer, const void* data, std::uint32_t offset, std::uint32_t size) = 0;

    virtual RenderPipeline* CreateRenderPipeline(const RenderPipelineDescriptor& descriptor) = 0;

    virtual Shader* CreateShader(const std::vector<std::uint8_t>& shaderSource) = 0;

    virtual Texture* CreateTexture(const TextureDescriptor& descriptor) = 0;

    virtual Sampler* CreateSampler(const SamplerDescriptor& descriptor = {}) = 0;
    
    virtual BindGroupLayout* CreateBindGroupLayout(const BindGroupLayoutDescriptor& descriptor) = 0;
    
    virtual BindGroup* CreateBindGroup(const BindGroupDescriptor& descriptor) = 0;

    virtual PipelineLayout* CreatePipelineLayout(const PipelineLayoutDescriptor& descriptor) = 0;

    virtual BindingResource* CreateBindingResourceBuffer(std::uint32_t bindingPoint, const Buffer* buffer, std::uint32_t offset, std::uint32_t size) = 0;

    virtual BindingResource* CreateBindingResourceCombined(std::uint32_t bindingPoint, const TextureView* textureView, const Sampler* sampler) = 0;
    
    virtual CommandEncoder* CreateCommandEncoder(const CommandEncoderDescriptor& descriptor = {}) = 0;

    virtual SwapChain* CreateSwapChain() = 0;
    
    virtual Viewport GetViewport() const = 0;

    virtual Scissor  GetScissor()  const = 0;

    virtual Queue*   GetQueue() const = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //ALITA_DEVICE_H
