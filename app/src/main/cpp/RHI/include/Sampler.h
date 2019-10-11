//
// Created by realxie on 2019-10-07.
//

#ifndef ALITA_SAMPLER_H
#define ALITA_SAMPLER_H

#include "Macros.h"
#include "RHIObjectBase.h"



NS_RHI_BEGIN

class Sampler;

class Sampler : public  RHIObjectBase
{
public:
    virtual ~Sampler()
    {
    }
};

NS_RHI_END

#endif //ALITA_SAMPLER_H
