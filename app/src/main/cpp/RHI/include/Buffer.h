//
// Created by realxie on 2019-10-02.
//

#ifndef VULKANDEMO_BUFFER_H
#define VULKANDEMO_BUFFER_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class Buffer;

typedef std::shared_ptr<Buffer> BufferHnd;

class Buffer
{
public:
    ~Buffer(){}
};

NS_RHI_END

#endif //VULKANDEMO_BUFFER_H
