//
// Created by realxie on 2019-10-02.
//

#ifndef VULKANDEMO_DEVICE_H
#define VULKANDEMO_DEVICE_H

#include "Macros.h"
#include "Buffer.h"
#include "Flags.h"

NS_RHI_BEGIN

class Device
{
public:
    virtual BufferHnd CreateBuffer(BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, void* data) = 0;
    virtual void CreateShaderModule(const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader) = 0;

    virtual void BeginRenderpass() = 0;
    virtual void EndRenderpass() = 0;
public:
    ~Device(){}
};

NS_RHI_END

#endif //VULKANDEMO_DEVICE_H
