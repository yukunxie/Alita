//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_RENDERPASS_H
#define ALITA_RENDERPASS_H

#include "Macros.h"
#include <memory>

NS_RHI_BEGIN

class RenderPass;

typedef std::shared_ptr<RenderPass> RenderPassHnd;

class RenderPass
{
public:
    virtual ~RenderPass(){}
};

NS_RHI_END

#endif //ALITA_RENDERPASS_H
