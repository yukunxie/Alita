//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKPIPELINELAYOUT_H
#define ALITA_VKPIPELINELAYOUT_H

#include "VKDevice.h"
#include "VKBindGroupLayout.h"

#include "../../include/Macros.h"
#include "../../include/PipelineLayout.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKPipelineLayout : public PipelineLayout
{
public:
    VKPipelineLayout(VKDevice* device, const std::vector<VKBindGroupLayout*> bindGroupLayouts);
    ~VKPipelineLayout();

    VkPipelineLayout GetNative() const {return vkPipelineLayout_;}

private:
    VkDevice            vkDevice_           = nullptr;
    VkPipelineLayout    vkPipelineLayout_   = 0;
};

NS_RHI_END




#endif //ALITA_VKPIPELINELAYOUT_H
