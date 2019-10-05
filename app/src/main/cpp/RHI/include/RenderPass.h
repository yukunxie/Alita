//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_RENDERPASS_H
#define VULKANDEMO_RENDERPASS_H

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

#endif //VULKANDEMO_RENDERPASS_H
