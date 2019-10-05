//
// Created by realxie on 2019-10-04.
//

#include "VKGraphicPipeline.h"
#include "VKTypes.h"
#include "VKShader.h"

#include <array>

NS_RHI_BEGIN

VKGraphicPipeline::VKGraphicPipeline(VKDevice* device, const std::vector<RHI::PipelineShaderStageCreateInfo>& shaderStageInfos
        , const PipelineVertexInputStateCreateInfo& vertexInputInfo
        , const PipelineViewportStateCreateInfo& viewportStateCreateInfo)
{
    VkDevice vkDevice = device->GetVulkanDevice();

    // Setup shader modules
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages (shaderStageInfos.size());
    for (size_t i = 0; i < shaderStageInfos.size(); ++i)
    {
        const auto& src = shaderStageInfos[i];
        auto& dst = shaderStages[i];
        dst.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        dst.stage = ToVkShaderStageFlagBits(src.stage);
        dst.flags = src.flags;
        dst.module = ((VKShader*)src.shader)->GetNative();
        dst.pName = src.entryName.c_str();
    }

    // setup vertex attribute input info

    std::vector<VkVertexInputBindingDescription> bindingDescriptions(vertexInputInfo.vertexBindingDescriptions.size());
    for (size_t i = 0; i < vertexInputInfo.vertexBindingDescriptions.size(); ++i)
    {
        const auto& src = vertexInputInfo.vertexBindingDescriptions[i];
        auto& dst = bindingDescriptions[i];
        dst.binding = src.binding;
        dst.stride = src.stride;
        dst.inputRate = ToVkVertexInputRate(src.inputRate);
    }

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(vertexInputInfo.vertexAttributeDescriptions.size());
    for (size_t i = 0; i < vertexInputInfo.vertexAttributeDescriptions.size(); ++i)
    {
        const auto& src = vertexInputInfo.vertexAttributeDescriptions[i];
        auto& dst = attributeDescriptions[i];
        dst.binding = src.binding;
        dst.location = src.location;
        dst.offset = src.offset;
        dst.format = ToVkFormat(src.format);
    }

    VkPipelineVertexInputStateCreateInfo vertexInputCreateDescription = {
            .vertexBindingDescriptionCount = (std::uint32_t)bindingDescriptions.size(),
            .pVertexBindingDescriptions = bindingDescriptions.data(),
            .vertexAttributeDescriptionCount = (std::uint32_t)attributeDescriptions.size(),
            .pVertexAttributeDescriptions = attributeDescriptions.data()
    };

    // setup ..

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
    };

    // setup viewport

    std::vector<VkViewport> viewports(viewportStateCreateInfo.viewports.size());
    for (size_t i = 0; i < viewportStateCreateInfo.viewports.size(); ++i)
    {
        const auto& src = viewportStateCreateInfo.viewports[i];
        auto& dst = viewports[i];
        dst.x = src.x;
        dst.y = src.y;
        dst.width  = src.width;
        dst.height = src.height;
        dst.minDepth = src.minDepth;
        dst.maxDepth = src.maxDepth;
    }

    std::vector<VkRect2D> scissors(viewportStateCreateInfo.scissors.size());
    for (size_t i = 0; i < viewportStateCreateInfo.scissors.size(); ++i)
    {
        const auto &src = viewportStateCreateInfo.scissors[i];
        auto &dst = scissors[i];
        dst.offset.x = src.x;
        dst.offset.y = src.y;
        dst.extent.width = src.width;
        dst.extent.height = src.height;
    }

    VkPipelineViewportStateCreateInfo viewportState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = (std::uint32_t)viewports.size(),
            .pViewports = viewports.data(),
            .scissorCount = (std::uint32_t)scissors.size(),
            .pScissors = scissors.data(),
    };

    // setup rasterization info

    VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .lineWidth = 1.0f,
            .cullMode = VK_CULL_MODE_NONE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,    // Optional
            .depthBiasClamp = 0.0f,             // Optional
            .depthBiasSlopeFactor = 0.0f,       // Optional
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f, // Optional
            .pSampleMask = nullptr, // Optional
            .alphaToCoverageEnable = VK_FALSE, // Optional
            .alphaToOneEnable = VK_FALSE, // Optional
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE,

            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
            .colorBlendOp = VK_BLEND_OP_ADD, // Optional
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
            .alphaBlendOp = VK_BLEND_OP_ADD, // Optional

            //            .blendEnable = VK_TRUE,
            //            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            //            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            //            .colorBlendOp = VK_BLEND_OP_ADD,
            //            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            //            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            //            .alphaBlendOp = VK_BLEND_OP_ADD,
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY, // Optional
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants[0] = 0.0f, // Optional
            .blendConstants[1] = 0.0f, // Optional
            .blendConstants[2] = 0.0f, // Optional
            .blendConstants[3] = 0.0f, // Optional
    };

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

    VkPipelineDynamicStateCreateInfo dynamicState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = 2,
            .pDynamicStates = dynamicStates,
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0, // Optional
            .pSetLayouts = nullptr, // Optional
            .pushConstantRangeCount = 0, // Optional
            .pPushConstantRanges = nullptr, // Optional
    };

    if (vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = (std::uint32_t)shaderStages.size(),
            .pStages = shaderStages.data(),
            .pVertexInputState = &vertexInputCreateDescription,
            .pInputAssemblyState = &inputAssembly,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            pipelineInfo.pDepthStencilState = nullptr, // Optional
            .pColorBlendState = &colorBlending,
            .pDynamicState = nullptr, // Optional

            .layout = vkPipelineLayout_,
            .renderPass = device->GetRenderPass(),
            .subpass = 0,

            .basePipelineHandle = VK_NULL_HANDLE, // Optional
            .basePipelineIndex = -1, // Optional
    };

    if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!"); }
}

VKGraphicPipeline::~VKGraphicPipeline()
{
    // TODO release vulkan resource.
}

void VKGraphicPipeline::Bind(VkCommandBuffer cmdBuffer)
{
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);
}

NS_RHI_END
