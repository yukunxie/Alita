//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_VKGRAPHICPIPELINE_H
#define ALITA_VKGRAPHICPIPELINE_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/GraphicPipleine.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>
#include <vector>

NS_RHI_BEGIN

class VKRenderPass;

class VKGraphicPipeline : public GraphicPipeline
{
public:
    VKGraphicPipeline(VKDevice* device, const GraphicPipelineCreateInfo& graphicPipelineCreateInfo);
    virtual ~VKGraphicPipeline();

    void Bind(VkCommandBuffer cmdBuffer);

    VkPipeline GetNative() const {return vkGraphicsPipeline_;}

    VkPipelineLayout GetPipelineLayout() const {return vkPipelineLayout_;}

private:

    void CreateRenderPass(VKDevice* device);

private:
    VkPipeline                      vkGraphicsPipeline_     = 0;
    VkPipelineLayout                vkPipelineLayout_       = 0;
    VKRenderPass*                   renderPass_             = nullptr;

    friend class VKDevice;
};

NS_RHI_END


#endif //ALITA_VKGRAPHICPIPELINE_H
