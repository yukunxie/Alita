//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_BINDINGRESOURCE_H
#define ALITA_BINDINGRESOURCE_H

#include "Macros.h"
#include "../../../../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdint"
#include <memory>

NS_RHI_BEGIN

class BindingResource;

enum class BindingResourceType
{
    UNDEFINED,
    BUFFER,
    SAMPLER,
    TEXTURE_VIEW,
    COMBINED_SAMPLER_TEXTUREVIEW,
};

typedef std::shared_ptr<BindingResource> BindingResourceHnd;

class BindingResource
{
public:
    BindingResource(BindingResourceType bindingResourceType, std::uint32_t bindingPoint = 0)
        : bindingResourceType_(bindingResourceType)
        , bindingPoint_(bindingPoint)
    {
    }

    virtual ~BindingResource()
    {
    }

    BindingResourceType GetResourceType() {return bindingResourceType_;}

    std::uint32_t GetBindingPoint() const {return bindingPoint_;}

protected:
    BindingResourceType bindingResourceType_;
    std::uint32_t       bindingPoint_;
};

NS_RHI_END

#endif //ALITA_BINDINGRESOURCE_H
