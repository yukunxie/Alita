//
// Created by realxie on 2019-10-14.
//

#include "VKCommandBuffer.h"

NS_RHI_BEGIN

VKCommandBuffer::VKCommandBuffer(VKDevice* device)
{
    vkDevice_ = device->GetDevice();
    vkCommandBuffer_ = device->GetCommandBuffer();
}

VKCommandBuffer::~VKCommandBuffer()
{
}


NS_RHI_END