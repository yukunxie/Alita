//
// Created by realxie on 2019-10-07.
//

#ifndef ALITA_VKSAMPLER_H
#define ALITA_VKSAMPLER_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/Sampler.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKSampler final : public Sampler
{
protected:
    VKSampler() = default;

public:
    bool Init(VKDevice* device, const SamplerDescriptor& descriptor);

    virtual ~VKSampler();

    VkSampler  GetNative() const {return vkSampler_;}

private:
    VkDevice  vkDevice_   = nullptr;
    VkSampler vkSampler_  = 0;

    friend class VKDevice;
};

NS_RHI_END


#endif //ALITA_VKSAMPLER_H
