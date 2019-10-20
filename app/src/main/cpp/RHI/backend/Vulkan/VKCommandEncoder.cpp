//
// Created by realxie on 2019-10-14.
//

#include "VKCommandEncoder.h"
#include "VKCommandBuffer.h"
#include "VKRenderPassEncoder.h"

NS_RHI_BEGIN

VKCommandEncoder::VKCommandEncoder(VKDevice* device)
{
    device_ = device;
    commandBuffer_ = new VKCommandBuffer(device_);
    RHI_SAFE_RETAIN(commandBuffer_);

    renderPassEncoder_ = new VKRenderPassEncoder();
    RHI_SAFE_RETAIN(renderPassEncoder_);
}

VKCommandEncoder::~VKCommandEncoder()
{
    RHI_SAFE_RELEASE(commandBuffer_);
    RHI_SAFE_RELEASE(renderPassEncoder_);
}

RenderPassEncoder* VKCommandEncoder::BeginRenderPass(/*GPURenderPassDescriptor descriptor*/)
{
    renderPassEncoder_->BeginPass(commandBuffer_->GetNative());
    return renderPassEncoder_;
}

CommandBuffer* VKCommandEncoder::Finish()
{
    return commandBuffer_;
}

NS_RHI_END