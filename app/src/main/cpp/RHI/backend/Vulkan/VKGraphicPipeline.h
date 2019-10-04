//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_VKGRAPHICPIPELINE_H
#define VULKANDEMO_VKGRAPHICPIPELINE_H

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
    VKGraphicPipeline(VKDevice* device, const std::vector<std::uint8_t>& vertexShader, const std::vector<std::uint8_t>& fragShader);
    virtual ~VKGraphicPipeline();

    void Bind(VkCommandBuffer cmdBuffer);

private:
    VkPipeline                      vkGraphicsPipeline_; // tmp
    VkPipelineLayout                vkPipelineLayout_; // tmp

    friend class VKDevice;
};

NS_RHI_END


#endif //VULKANDEMO_VKGRAPHICPIPELINE_H
