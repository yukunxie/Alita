//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_GRAPHICPIPLEINE_H
#define ALITA_GRAPHICPIPLEINE_H

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

#endif //ALITA_GRAPHICPIPLEINE_H
