//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_BINDINGRESOURCE_H
#define ALITA_BINDINGRESOURCE_H

#include "Macros.h"
#include "RHIObjectBase.h"

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

class BindingResource : public  RHIObjectBase
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

    BindingResourceType GetResourceType() const {return bindingResourceType_;}

    std::uint32_t GetBindingPoint() const {return bindingPoint_;}

protected:
    BindingResourceType bindingResourceType_;
    std::uint32_t       bindingPoint_;
};

NS_RHI_END

#endif //ALITA_BINDINGRESOURCE_H
