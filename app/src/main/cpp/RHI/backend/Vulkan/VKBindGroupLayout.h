//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKBINDGROUPLAYOUT_H
#define ALITA_VKBINDGROUPLAYOUT_H

#include "VKDevice.h"

#include "../../include/Macros.h"
#include "../../include/BindGroupLayout.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN


class VKBindGroupLayout : public BindGroupLayout
{
public:
    VKBindGroupLayout(VKDevice* device, const DescriptorSetLayoutCreateInfo& layoutCreateInfo);
    virtual ~VKBindGroupLayout();

    VkDescriptorSetLayout GetNative() const {return vkBindGroupLayout_;}

private:
    VkDevice         vkDevice_          = nullptr;
    VkDescriptorSetLayout vkBindGroupLayout_  = 0L;
};

NS_RHI_END


#endif //ALITA_VKBINDGROUPLAYOUT_H
