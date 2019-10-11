//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_GRAPHICPIPLEINE_H
#define ALITA_GRAPHICPIPLEINE_H

#include "Macros.h"
#include "Flags.h"
#include "BindGroupLayout.h"
#include "PipelineLayout.h"

#include <memory>

NS_RHI_BEGIN

class GraphicPipeline;

typedef struct GraphicPipelineCreateInfo
{
    std::vector<PipelineShaderStageCreateInfo>  shaderStageInfos;
    PipelineVertexInputStateCreateInfo          vertexInputInfo;
    PipelineViewportStateCreateInfo             viewportState;
    const PipelineLayout*                       pPipelineLayout;

} GraphicPipelineCreateInfo;

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
