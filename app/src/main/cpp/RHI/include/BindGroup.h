//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_BINDGROUP_H
#define ALITA_BINDGROUP_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class BindGroup;

typedef std::shared_ptr<BindGroup> BindGroupHnd;

class BindGroup
{
public:
    virtual ~BindGroup(){}
};

NS_RHI_END

#endif //ALITA_BINDGROUP_H
