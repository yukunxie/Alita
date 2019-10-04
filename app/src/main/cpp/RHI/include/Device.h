//
// Created by realxie on 2019-10-02.
//

#ifndef VULKANDEMO_DEVICE_H
#define VULKANDEMO_DEVICE_H

#include "Macros.h"
#include "Buffer.h"
#include "GraphicPipleine.h"
#include "Shader.h"
#include "Flags.h"

NS_RHI_BEGIN

class Device
{
public:
    virtual BufferHnd CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data) = 0;
    virtual GraphicPipelineHnd CreateGraphicPipeline(const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader) = 0;
    virtual ShaderHnd CreateShader(const std::vector<std::uint8_t>& shaderSource) = 0;

    virtual void BindBuffer(BufferHnd buffer, std::uint32_t offset) = 0;
    virtual void Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) = 0;
    virtual void Draw(std::uint32_t vertexCount, std::uint32_t firstVertex) = 0;
    virtual void BindGraphicPipeline(GraphicPipelineHnd graphicPipeline) = 0;

    virtual void BeginRenderpass() = 0;
    virtual void EndRenderpass() = 0;

    virtual Viewport GetViewport() = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //VULKANDEMO_DEVICE_H
