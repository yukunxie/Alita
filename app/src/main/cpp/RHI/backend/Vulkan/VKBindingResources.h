//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKBINDINGRESOURCES_H
#define ALITA_VKBINDINGRESOURCES_H


#include "VKDevice.h"

#include "../../include/Macros.h"
#include "../../include/BindingResource.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKBindingBuffer : public BindingResource
{
public:
    VKBindingBuffer();
    ~VKBindingBuffer();
};

class VKBindingSampler : public BindingResource
{
public:
    VKBindingSampler();
    ~VKBindingSampler();
};

class VKBindingTextureView : public BindingResource
{
public:
    VKBindingTextureView();
    ~VKBindingTextureView();
};

// combined sampler and texture view
class VKBindingCombined : public BindingResource
{
public:
    VKBindingCombined();
    ~VKBindingCombined();
};

NS_RHI_END

#endif //ALITA_VKBINDINGRESOURCES_H
