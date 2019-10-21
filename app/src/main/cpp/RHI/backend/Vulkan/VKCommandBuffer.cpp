//
// Created by realxie on 2019-10-14.
//

#include "VKCommandBuffer.h"

NS_RHI_BEGIN

VKCommandBuffer::VKCommandBuffer(VKDevice* device)
    : device_(device)
{
    vkCommandBuffer_ = device->GetCommandBuffer();
}

VKCommandBuffer::~VKCommandBuffer()
{
}

void VKCommandBuffer::ResetCommandBuffer()
{
    vkCommandBuffer_ = device_->GetCommandBuffer();
}


NS_RHI_END