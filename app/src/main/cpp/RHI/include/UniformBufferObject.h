//
// Created by realxie on 2019-10-06.
//

#ifndef ALITA_UNIFORMBUFFEROBJECT_H
#define ALITA_UNIFORMBUFFEROBJECT_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class UniformBufferObject;

typedef std::shared_ptr<UniformBufferObject> UniformBufferObjectHnd;

class UniformBufferObject
{
public:
    virtual ~UniformBufferObject(){}
};

NS_RHI_END

#endif //ALITA_UNIFORMBUFFEROBJECT_H
