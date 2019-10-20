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

    void Reset(VkSwapchainKHR vkSwapchain, std::uint32_t imageIndex) {
        vkSwapchain_ = vkSwapchain;
        imageIndex_ = imageIndex;
    }

public:
    virtual void Submit(CommandBuffer* commandBuffer) override;

private:
    VkDevice  vkDevice_ = nullptr;
    VkQueue   vkQueue_  = 0L;
    VkFence   vkFence_  = 0L;
    VkSwapchainKHR  vkSwapchain_    = 0;
    std::uint32_t   imageIndex_     = 0;

};

NS_RHI_END


#endif //ALITA_VKQUEUE_H
