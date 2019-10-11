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
};

NS_RHI_END

#endif //ALITA_BUFFER_H
