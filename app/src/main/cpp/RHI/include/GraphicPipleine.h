//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_GRAPHICPIPLEINE_H
#define VULKANDEMO_GRAPHICPIPLEINE_H

#include "Macros.h"

#include <memory>

NS_RHI_BEGIN

class GraphicPipeline;

typedef std::shared_ptr<GraphicPipeline> GraphicPipelineHnd;

class GraphicPipeline
{
public:
    virtual ~GraphicPipeline()
    {
    }
};

NS_RHI_END

#endif //VULKANDEMO_GRAPHICPIPLEINE_H
