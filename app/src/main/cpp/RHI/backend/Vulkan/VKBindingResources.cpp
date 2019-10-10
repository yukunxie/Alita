//
// Created by realxie on 2019-10-10.
//

#include "VKBindingResources.h"


NS_RHI_BEGIN

VKBindingBuffer::VKBindingBuffer()
    : BindingResource(BindingResourceType::BUFFER)
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

VKBindingCombined::VKBindingCombined()
        : BindingResource(BindingResourceType::COMBINED_SAMPLER_TEXTUREVIEW)
{

}

VKBindingCombined::~VKBindingCombined()
{

}


NS_RHI_END