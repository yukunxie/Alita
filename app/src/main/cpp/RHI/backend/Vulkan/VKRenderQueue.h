//
// Created by realxie on 2019-10-11.
//

#ifndef ALITA_VKRENDERQUEUE_H
#define ALITA_VKRENDERQUEUE_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/RenderQueue.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKRenderQueue : public RenderQueue
{
public:
    VKRenderQueue();
    virtual ~VKRenderQueue();
};

NS_RHI_END


#endif //ALITA_VKRENDERQUEUE_H
