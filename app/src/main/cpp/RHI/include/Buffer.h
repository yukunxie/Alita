//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_BUFFER_H
#define ALITA_BUFFER_H

#include "Macros.h"
#include "RHIObjectBase.h"



NS_RHI_BEGIN

class Buffer;

class Buffer : public RHIObjectBase
{
protected:
    virtual ~Buffer(){}

public:
    virtual const void* MapReadAsync(std::uint32_t offset, std::uint32_t size) = 0;
    virtual void* MapWriteAsync(std::uint32_t offset, std::uint32_t size)= 0;
    virtual const void* MapReadAsync() = 0;
    virtual void* MapWriteAsync()= 0;
    virtual void Unmap() = 0;
    virtual void Destroy() = 0;
    virtual void SetSubData(std::uint32_t offset, std::uint32_t byteSize, const void* data) = 0;
};

NS_RHI_END

#endif //ALITA_BUFFER_H
