//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKBINDGROUP_H
#define ALITA_VKBINDGROUP_H

#include "VKDevice.h"
#include "VKBindGroupLayout.h"
#include "VKBindingResources.h"

#include "../../include/Macros.h"
#include "../../include/BindGroup.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"



#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKBindGroup : public BindGroup
{
public:
    VKBindGroup(VKDevice* device, const VKBindGroupLayout* bindGroupLayout, const std::vector<BindingResource*>& bindResources);
    ~VKBindGroup();

    VkDescriptorSet GetNative() const {return vkDescriptorSet_;}

    void WriteToGPU() const;

    void BindToCommandBuffer(std::uint32_t index, VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout) const;

private:
    VkDevice                    vkDevice_           = nullptr;
    VkDescriptorSet             vkDescriptorSet_    = 0L;

    std::vector<const BindingResource*> bindingResources_;
};

NS_RHI_END


#endif //ALITA_VKBINDGROUP_H
