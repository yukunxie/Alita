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

class VKGraphicPipeline : public GraphicPipeline
{
public:
    VKGraphicPipeline(VKDevice* device, const std::vector<RHI::PipelineShaderStageCreateInfo>& shaderStageInfos
            , const PipelineVertexInputStateCreateInfo& vertexInputInfo
            , const PipelineViewportStateCreateInfo& viewportState);
    virtual ~VKGraphicPipeline();

    void Bind(VkCommandBuffer cmdBuffer);

    VkPipeline GetPipeline() const {return vkGraphicsPipeline_;}

    VkPipelineLayout GetPipelineLayout() const {return vkPipelineLayout_;}

    VkDescriptorSetLayout GetDescriptorSetLayout() const {return vkDescriptorSetLayout_;}

    VkDescriptorSet GetDescriptorSet() const {return vkDescriptorSet_;}

private:
    VkPipeline                      vkGraphicsPipeline_     = 0;
    VkPipelineLayout                vkPipelineLayout_       = 0;
    VkDescriptorSetLayout           vkDescriptorSetLayout_  = 0;
    VkDescriptorSet                 vkDescriptorSet_        = 0;


    friend class VKDevice;
};

NS_RHI_END


#endif //ALITA_VKGRAPHICPIPELINE_H
