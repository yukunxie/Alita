//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_BUFFER_H
#define ALITA_BUFFER_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class Buffer;

typedef std::shared_ptr<Buffer> BufferHnd;

class Buffer
{
public:
    virtual ~Buffer(){}
};

NS_RHI_END

#endif //ALITA_BUFFER_H
