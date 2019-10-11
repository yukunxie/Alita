//
// Created by realxie on 2019-10-10.
//

#include "VKBindingResources.h"


NS_RHI_BEGIN

VKBindingBuffer::VKBindingBuffer(std::uint32_t bindingPoint, const VKBuffer* buffer, std::uint32_t offset, std::uint32_t size)
    : BindingResource(BindingResourceType::BUFFER, bindingPoint)
    , buffer_(buffer)
    , offset_(offset)
    , size_  (size)
{
}

VKBindingBuffer::~VKBindingBuffer()
{

}

VKBindingSampler::VKBindingSampler()
        : BindingResource(BindingResourceType::SAMPLER)
{

}

VKBindingSampler::~VKBindingSampler()
{

}


VKBindingTextureView::VKBindingTextureView()
        : BindingResource(BindingResourceType::TEXTURE_VIEW)
{

}

VKBindingTextureView::~VKBindingTextureView()
{

}

VKBindingCombined::VKBindingCombined(std::uint32_t bindingPoint, const VKTextureView* textureView, const VKSampler* sampler)
        : BindingResource(BindingResourceType::COMBINED_SAMPLER_TEXTUREVIEW, bindingPoint)
        , textureView_(textureView)
        , sampler_ (sampler)
{

}

VKBindingCombined::~VKBindingCombined()
{

}


NS_RHI_END