//
// Created by realxie on 2019-10-10.
//

#ifndef ALITA_BINDGROUPLAYOUT_H
#define ALITA_BINDGROUPLAYOUT_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class BindGroupLayout;

typedef std::shared_ptr<BindGroupLayout> BindGroupLayoutHnd;

class BindGroupLayout
{
public:
    virtual ~BindGroupLayout(){}
};

NS_RHI_END

#endif //ALITA_BINDGROUPLAYOUT_H
