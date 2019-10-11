//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_VKBINDINGRESOURCES_H
#define ALITA_VKBINDINGRESOURCES_H


#include "VKDevice.h"
#include "VKSampler.h"
#include "VKTextureView.h"

#include "../../include/Macros.h"
#include "../../include/BindingResource.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKBindGroup;

class VKBindingBuffer final : public BindingResource
{
public:
    VKBindingBuffer(std::uint32_t bindingPoint, const VKBuffer* buffer, std::uint32_t offset, std::uint32_t size);
    virtual ~VKBindingBuffer();

protected:
    const VKBuffer*     buffer_             = nullptr;
    std::uint32_t       offset_             = 0;
    std::uint32_t       size_               = 0;

    friend class VKBindGroup;
};

class VKBindingSampler final: public BindingResource
{
public:
    VKBindingSampler();
    virtual ~VKBindingSampler();
};

class VKBindingTextureView final: public BindingResource
{
public:
    VKBindingTextureView();
    virtual ~VKBindingTextureView();

    friend class VKBindGroup;
};

// combined sampler and texture view
class VKBindingCombined final: public BindingResource
{
public:
    VKBindingCombined(std::uint32_t bindingPoint, const VKTextureView* textureView, const VKSampler* sampler);
    virtual ~VKBindingCombined();

protected:
    const VKTextureView*    textureView_  = nullptr;
    const VKSampler*        sampler_      = nullptr;

    friend class VKBindGroup;
};

NS_RHI_END

#endif //ALITA_VKBINDINGRESOURCES_H
