//
// Created by realxie on 2019-10-18.
//

#ifndef ALITA_VKQUEUE_H
#define ALITA_VKQUEUE_H

#include "VKDevice.h"

#include "../../include/Macros.h"
#include "../../include/Queue.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKQueue : public Queue
{
public:
    VKQueue(VKDevice* device);
    virtual ~VKQueue();

public:
    virtual void Submit(CommandBuffer* commandBuffer) override;

private:
    VKDevice* device_   = nullptr;
    VkDevice  vkDevice_ = nullptr;
    VkQueue   vkQueue_  = 0L;
    VkFence   vkFence_  = 0L;
};

NS_RHI_END


#endif //ALITA_VKQUEUE_H
