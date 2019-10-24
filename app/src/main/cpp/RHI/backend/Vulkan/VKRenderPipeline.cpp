//
// Created by realxie on 2019-10-04.
//

#include "VKRenderPipeline.h"
#include "VKTypes.h"
#include "VKShader.h"
#include "VKBindGroupLayout.h"
#include "VKPipelineLayout.h"
#include "VKRenderPass.h"

#include <array>

NS_RHI_BEGIN

//VKRenderPipeline::VKRenderPipeline(VKDevice* device, const GraphicPipelineCreateInfo& graphicPipelineCreateInfo)
//{
//    VkDevice vkDevice = device->GetDevice();
//
//    CreateRenderPass(device);
//
//    const std::vector<RHI::PipelineShaderStageCreateInfo>& shaderStageInfos = graphicPipelineCreateInfo.shaderStageInfos;
//    const PipelineVertexInputStateCreateInfo& vertexInputInfo = graphicPipelineCreateInfo.vertexInputInfo;
//    const PipelineViewportStateCreateInfo& viewportStateCreateInfo = graphicPipelineCreateInfo.viewportState;
//
//    vkPipelineLayout_ = ((const VKPipelineLayout*)graphicPipelineCreateInfo.pPipelineLayout)->GetNative();
//
//    // Setup shader modules
//    std::vector<VkPipelineShaderStageCreateInfo> shaderStages (shaderStageInfos.size());
//    for (size_t i = 0; i < shaderStageInfos.size(); ++i)
//    {
//        const auto& src = shaderStageInfos[i];
//        auto& dst = shaderStages[i];
//        dst.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//        dst.stage = ToVkShaderStageFlagBits(src.stage);
//        dst.flags = src.flags;
//        dst.module = ((VKShader*)src.shader)->GetNative();
//        dst.pName = src.entryName.c_str();
//    }
//
//    // setup vertex attribute input info
//
//    std::vector<VkVertexInputBindingDescription> bindingDescriptions(vertexInputInfo.vertexBindingDescriptions.size());
//    for (size_t i = 0; i < vertexInputInfo.vertexBindingDescriptions.size(); ++i)
//    {
//        const auto& src = vertexInputInfo.vertexBindingDescriptions[i];
//        auto& dst = bindingDescriptions[i];
//        dst.binding = src.binding;
//        dst.stride = src.stride;
//        dst.inputRate = ToVkVertexInputRate(src.inputRate);
//    }
//
//    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(vertexInputInfo.vertexAttributeDescriptions.size());
//    for (size_t i = 0; i < vertexInputInfo.vertexAttributeDescriptions.size(); ++i)
//    {
//        const auto& src = vertexInputInfo.vertexAttributeDescriptions[i];
//        auto& dst = attributeDescriptions[i];
//        dst.binding = src.binding;
//        dst.location = src.location;
//        dst.offset = src.offset;
//        dst.format = ToVkFormat(src.format);
//    }
//
//    VkPipelineVertexInputStateCreateInfo vertexInputCreateDescription = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
//            .vertexBindingDescriptionCount = (std::uint32_t)bindingDescriptions.size(),
//            .pVertexBindingDescriptions = bindingDescriptions.data(),
//            .vertexAttributeDescriptionCount = (std::uint32_t)attributeDescriptions.size(),
//            .pVertexAttributeDescriptions = attributeDescriptions.data()
//    };
//
//    // setup ..
//
//    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
//            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
//            .primitiveRestartEnable = VK_FALSE
//    };
//
//    // setup viewport
//
//    std::vector<VkViewport> viewports(viewportStateCreateInfo.viewports.size());
//    for (size_t i = 0; i < viewportStateCreateInfo.viewports.size(); ++i)
//    {
//        const auto& src = viewportStateCreateInfo.viewports[i];
//        auto& dst = viewports[i];
//        dst.x = src.x;
//        dst.y = src.y;
//        dst.width  = src.width;
//        dst.height = src.height;
//        dst.minDepth = src.minDepth;
//        dst.maxDepth = src.maxDepth;
//    }
//
//    std::vector<VkRect2D> scissors(viewportStateCreateInfo.scissors.size());
//    for (size_t i = 0; i < viewportStateCreateInfo.scissors.size(); ++i)
//    {
//        const auto &src = viewportStateCreateInfo.scissors[i];
//        auto &dst = scissors[i];
//        dst.offset.x = src.x;
//        dst.offset.y = src.y;
//        dst.extent.width = src.width;
//        dst.extent.height = src.height;
//    }
//
//    VkPipelineViewportStateCreateInfo viewportState = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
//            .viewportCount = (std::uint32_t)viewports.size(),
//            .pViewports = viewports.data(),
//            .scissorCount = (std::uint32_t)scissors.size(),
//            .pScissors = scissors.data(),
//    };
//
//    // setup rasterization info
//
//    VkPipelineRasterizationStateCreateInfo rasterizer = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
//            .depthClampEnable = VK_TRUE,
//            .rasterizerDiscardEnable = VK_FALSE,
//            .polygonMode = VK_POLYGON_MODE_FILL,
//            .lineWidth = 1.0f,
//            .cullMode = VK_CULL_MODE_NONE,
//            .depthBiasEnable = VK_FALSE,
//            .depthBiasConstantFactor = 0.0f,    // Optional
//            .depthBiasClamp = 0.0f,             // Optional
//            .depthBiasSlopeFactor = 0.0f,       // Optional
//    };
//
//    VkPipelineMultisampleStateCreateInfo multisampling = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
//            .sampleShadingEnable = VK_FALSE,
//            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
//            .minSampleShading = 1.0f, // Optional
//            .pSampleMask = nullptr, // Optional
//            .alphaToCoverageEnable = VK_FALSE, // Optional
//            .alphaToOneEnable = VK_FALSE, // Optional
//    };
//
//    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
//            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
//            .blendEnable = VK_FALSE,
//
//            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
//            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
//            .colorBlendOp = VK_BLEND_OP_ADD, // Optional
//            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
//            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
//            .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
//
//            //            .blendEnable = VK_TRUE,
//            //            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
//            //            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
//            //            .colorBlendOp = VK_BLEND_OP_ADD,
//            //            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
//            //            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
//            //            .alphaBlendOp = VK_BLEND_OP_ADD,
//    };
//
//    VkPipelineColorBlendStateCreateInfo colorBlending = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
//            .logicOpEnable = VK_FALSE,
//            .logicOp = VK_LOGIC_OP_COPY, // Optional
//            .attachmentCount = 1,
//            .pAttachments = &colorBlendAttachment,
//            .blendConstants[0] = 0.0f, // Optional
//            .blendConstants[1] = 0.0f, // Optional
//            .blendConstants[2] = 0.0f, // Optional
//            .blendConstants[3] = 0.0f, // Optional
//    };
//
//    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
//
//    VkPipelineDynamicStateCreateInfo dynamicState = {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
//            .dynamicStateCount = 2,
//            .pDynamicStates = dynamicStates,
//    };
//
//    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo {
//            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
//            .depthTestEnable = VK_TRUE,
//            .depthWriteEnable = VK_TRUE,
//            .depthCompareOp = VK_COMPARE_OP_LESS,
//
//            .depthBoundsTestEnable = VK_TRUE,
//            .minDepthBounds = 0.0f, // Optional
//            .maxDepthBounds = 1.0f, // Optional
//
//            .stencilTestEnable = VK_FALSE,
//            .front = {}, // Optional
//            .back = {}, // Optional
//    };
//
//    VkGraphicsPipelineCreateInfo pipelineInfo = {
//            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
//            .stageCount = (std::uint32_t)shaderStages.size(),
//            .pStages = shaderStages.data(),
//            .pVertexInputState = &vertexInputCreateDescription,
//            .pInputAssemblyState = &inputAssembly,
//            .pViewportState = &viewportState,
//            .pRasterizationState = &rasterizer,
//            .pMultisampleState = &multisampling,
//            .pDepthStencilState = &depthStencilStateCreateInfo,
//            .pColorBlendState = &colorBlending,
//            .pDynamicState = nullptr, // Optional
//
//            .layout = vkPipelineLayout_,
//            .renderPass = renderPass_->GetNative(),
//            .subpass = 0,
//
//            .basePipelineHandle = VK_NULL_HANDLE, // Optional
//            .basePipelineIndex = -1, // Optional
//    };
//
//    if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS)
//    {
//        throw std::runtime_error("failed to create graphics pipeline!");
//    }
//}

static VkPrimitiveTopology GetPrimitiveTopology(PrimitiveTopology topology)
{
    return (VkPrimitiveTopology)topology;
}

static VkVertexInputRate GetVertexInputRate(InputStepMode mode)
{
    switch (mode)
    {
        case InputStepMode::VERTEX :
            return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
        case InputStepMode::INSTANCE:
            return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
    }
}

static std::uint32_t GetVertexFormatSize(VertexFormat format)
{
    switch (format)
    {
        case VertexFormat::UCHAR2 :
            return 2;
        case VertexFormat::UCHAR4 :
            return 4;
        case VertexFormat::CHAR2 :
            return 2;
        case VertexFormat::CHAR4 :
            return 4;
        case VertexFormat::UCHAR2NORM :
            return 2;
        case VertexFormat::UCHAR4NORM :
            return 4;
        case VertexFormat::CHAR2NORM :
            return 2;
        case VertexFormat::CHAR4NORM :
            return 4;
        case VertexFormat::USHORT2 :
            return 4;
        case VertexFormat::USHORT4 :
            return 8;
        case VertexFormat::SHORT2 :
            return 4;
        case VertexFormat::SHORT4 :
            return 8;
        case VertexFormat::USHORT2NORM :
            return 4;
        case VertexFormat::USHORT4NORM :
            return 8;
        case VertexFormat::SHORT2NORM :
            return 4;
        case VertexFormat::SHORT4NORM :
            return 8;
        case VertexFormat::HALF2 :
            return 4;
        case VertexFormat::HALF4 :
            return 8;
        case VertexFormat::FLOAT :
            return 4;
        case VertexFormat::FLOAT2 :
            return 8;
        case VertexFormat::FLOAT3 :
            return 12;
        case VertexFormat::FLOAT4 :
            return 16;
        case VertexFormat::UINT :
            return 4;
        case VertexFormat::UINT2 :
            return 8;
        case VertexFormat::UINT3 :
            return 12;
        case VertexFormat::UINT4 :
            return 16;
        case VertexFormat::INT :
            return 4;
        case VertexFormat::INT2 :
            return 8;
        case VertexFormat::INT3 :
            return 12;
        case VertexFormat::INT4 :
            return 16;
    }
}

static VkFormat GetVkFormat(VertexFormat format)
{
    switch (format)
    {
        case VertexFormat::UCHAR2 :
            return VkFormat::VK_FORMAT_R8G8_UINT;
        case VertexFormat::UCHAR4 :
            return VkFormat::VK_FORMAT_R8G8B8A8_UINT;
        case VertexFormat::CHAR2 :
            return VkFormat::VK_FORMAT_R8G8_SINT;
        case VertexFormat::CHAR4 :
            return VkFormat::VK_FORMAT_R8G8B8A8_SINT;
        case VertexFormat::UCHAR2NORM :
            return VkFormat::VK_FORMAT_R8G8_UNORM;
        case VertexFormat::UCHAR4NORM :
            return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
        case VertexFormat::CHAR2NORM :
            return VkFormat::VK_FORMAT_R8G8_SNORM;
        case VertexFormat::CHAR4NORM :
            return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
        case VertexFormat::USHORT2 :
            return VkFormat::VK_FORMAT_R16G16_UINT;
        case VertexFormat::USHORT4 :
            return VkFormat::VK_FORMAT_R16G16B16A16_UINT;
        case VertexFormat::SHORT2 :
            return VkFormat::VK_FORMAT_R16G16_SINT;
        case VertexFormat::SHORT4 :
            return VkFormat::VK_FORMAT_R16G16B16A16_SINT;
        case VertexFormat::USHORT2NORM :
            return VkFormat::VK_FORMAT_R16G16_UNORM;
        case VertexFormat::USHORT4NORM :
            return VkFormat::VK_FORMAT_R16G16B16A16_UNORM;
        case VertexFormat::SHORT2NORM :
            return VkFormat::VK_FORMAT_R16G16_SNORM;
        case VertexFormat::SHORT4NORM :
            return VkFormat::VK_FORMAT_R16G16B16A16_SNORM;
        case VertexFormat::HALF2 :
            return VkFormat::VK_FORMAT_R16G16_SFLOAT;
        case VertexFormat::HALF4 :
            return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
        case VertexFormat::FLOAT :
            return VkFormat::VK_FORMAT_R32_SFLOAT;
        case VertexFormat::FLOAT2 :
            return VkFormat::VK_FORMAT_R32G32_SFLOAT;
        case VertexFormat::FLOAT3 :
            return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
        case VertexFormat::FLOAT4 :
            return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
        case VertexFormat::UINT :
            return VkFormat::VK_FORMAT_R32_UINT;
        case VertexFormat::UINT2 :
            return VkFormat::VK_FORMAT_R32G32_UINT;
        case VertexFormat::UINT3 :
            return VkFormat::VK_FORMAT_R32G32B32_UINT;
        case VertexFormat::UINT4 :
            return VkFormat::VK_FORMAT_R32G32B32A32_UINT;
        case VertexFormat::INT :
            return VkFormat::VK_FORMAT_R32_SINT;
        case VertexFormat::INT2 :
            return VkFormat::VK_FORMAT_R32G32_SINT;
        case VertexFormat::INT3 :
            return VkFormat::VK_FORMAT_R32G32B32_SINT;
        case VertexFormat::INT4 :
            return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
    }
}

static VkBool32 GetVkBoolean(bool b)
{
    return b ? VK_TRUE : VK_FALSE;
}

static VkCompareOp GetCompareOp(CompareFunction compareFunction)
{
    switch (compareFunction)
    {
        case CompareFunction::NEVER :
            return VkCompareOp::VK_COMPARE_OP_NEVER;
        case CompareFunction::LESS :
            return VkCompareOp::VK_COMPARE_OP_LESS;
        case CompareFunction::EQUAL :
            return VkCompareOp::VK_COMPARE_OP_EQUAL;
        case CompareFunction::LESS_EQUAL :
            return VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareFunction::GREATER :
            return VkCompareOp::VK_COMPARE_OP_GREATER;
        case CompareFunction::NOT_EQUAL :
            return VkCompareOp::VK_COMPARE_OP_NOT_EQUAL;
        case CompareFunction::GREATER_EQUAL :
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareFunction::ALWAYS :
            return VK_COMPARE_OP_ALWAYS;
    }
}

VKRenderPipeline::VKRenderPipeline(VKDevice* device, const RenderPipelineDescriptor& descriptor)
{
    vkPipelineLayout_ = RHI_CAST(const VKPipelineLayout*, descriptor.layout)->GetNative();

    VkPipelineShaderStageCreateInfo shaderStages[2];
    {
        const auto& vertexStageInfo = descriptor.vertexStage;
        const auto& fragmentStageInfo = descriptor.fragmentStage;

        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].flags = 0;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = RHI_CAST(const VKShader*, vertexStageInfo.module)->GetNative();
        shaderStages[0].pName = vertexStageInfo.entryPoint.c_str();
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].flags = 0;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = RHI_CAST(const VKShader*, fragmentStageInfo.module)->GetNative();
        shaderStages[1].pName = fragmentStageInfo.entryPoint.c_str();
        shaderStages[1].pSpecializationInfo = nullptr;
    }

    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.pNext = nullptr;
        inputAssembly.flags = 0;
        inputAssembly.topology = GetPrimitiveTopology(descriptor.primitiveTopology);
        // Primitive restart is always enabled in Dawn (because of Metal)
        inputAssembly.primitiveRestartEnable = VK_TRUE;
    }

    // A dummy viewport/scissor info. The validation layers force use to provide at least one
    // scissor and one viewport here, even if we choose to make them dynamic.
    VkViewport viewportDesc;
    {
        viewportDesc.x = 0.0f;
        viewportDesc.y = 0.0f;
        viewportDesc.width = 1.0f;
        viewportDesc.height = 1.0f;
        viewportDesc.minDepth = 0.0f;
        viewportDesc.maxDepth = 1.0f;
    }

    VkRect2D scissorRect;
    {
        scissorRect.offset.x = 0;
        scissorRect.offset.y = 0;
        scissorRect.extent.width = 1;
        scissorRect.extent.height = 1;
    }

    VkPipelineViewportStateCreateInfo viewport;
    {
        viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport.pNext = nullptr;
        viewport.flags = 0;
        viewport.viewportCount = 1;
        viewport.pViewports = &viewportDesc;
        viewport.scissorCount = 1;
        viewport.pScissors = &scissorRect;
    }


    VkPipelineRasterizationStateCreateInfo rasterization;
    {
        rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization.pNext = nullptr;
        rasterization.flags = 0;
        rasterization.depthClampEnable = VK_FALSE;
        rasterization.rasterizerDiscardEnable = VK_FALSE;
        rasterization.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization.cullMode = VK_CULL_MODE_NONE;
        rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterization.depthBiasEnable = VK_FALSE;
        rasterization.depthBiasConstantFactor = 0.0f;
        rasterization.depthBiasClamp = 0.0f;
        rasterization.depthBiasSlopeFactor = 0.0f;
        rasterization.lineWidth = 1.0f;
    }

    VkPipelineMultisampleStateCreateInfo multisample;
    {
        multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample.pNext = nullptr;
        multisample.flags = 0;
        multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample.sampleShadingEnable = VK_FALSE;
        multisample.minSampleShading = 0.0f;
        multisample.pSampleMask = nullptr;
        multisample.alphaToCoverageEnable = VK_FALSE;
        multisample.alphaToOneEnable = VK_FALSE;
    }

    // TODO realxie configurable

    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        // colorBlendAttachment.blendEnable = VK_TRUE;
        // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        // colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo colorBlending;
    {
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.flags = 0;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional
    };

    // Tag all state as dynamic but stencil masks.
    VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,          VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_LINE_WIDTH,        VK_DYNAMIC_STATE_DEPTH_BIAS,
            VK_DYNAMIC_STATE_BLEND_CONSTANTS,   VK_DYNAMIC_STATE_DEPTH_BOUNDS,
            VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };

    VkPipelineDynamicStateCreateInfo dynamic;
    {
        dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic.pNext = nullptr;
        dynamic.flags = 0;
        dynamic.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
        dynamic.pDynamicStates = dynamicStates;
    }

    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    for (const auto& vertexBuffer: descriptor.vertexInput.vertexBuffers)
    {
        // Notice, vertex binding must be start with 0
        VkVertexInputBindingDescription bindingDescription;
        {
            bindingDescription.binding   = bindingDescriptions.size();
            bindingDescription.inputRate = GetVertexInputRate(vertexBuffer.stepMode);
            bindingDescription.stride    = vertexBuffer.stride;
        }
        bindingDescriptions.push_back(bindingDescription);

        for (const auto& attriDescriptor : vertexBuffer.attributeSet)
        {
            VkVertexInputAttributeDescription attributeDescription;
            {
                attributeDescription.binding  = bindingDescriptions.size() - 1;
                attributeDescription.location = attriDescriptor.shaderLocation;
                attributeDescription.offset   = attriDescriptor.offset;
                attributeDescription.format   = GetVkFormat(attriDescriptor.format);
            }
            attributeDescriptions.push_back(attributeDescription);
        }
    }

    VkPipelineVertexInputStateCreateInfo vertexInputCreateDescription;
    {
        vertexInputCreateDescription.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputCreateDescription.flags  = 0;
        vertexInputCreateDescription.vertexBindingDescriptionCount = (std::uint32_t)bindingDescriptions.size();
        vertexInputCreateDescription.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputCreateDescription.vertexAttributeDescriptionCount = (std::uint32_t)attributeDescriptions.size();
        vertexInputCreateDescription.pVertexAttributeDescriptions = attributeDescriptions.data();
    };

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
    {
        const DepthStencilStateDescriptor& dsDescriptor = descriptor.depthStencilState;

        depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateCreateInfo.flags = 0;
        depthStencilStateCreateInfo.depthTestEnable  = dsDescriptor.depthCompare != CompareFunction::ALWAYS ? VK_TRUE : VK_FALSE;
        depthStencilStateCreateInfo.depthWriteEnable = GetVkBoolean(dsDescriptor.depthWriteEnabled);
        depthStencilStateCreateInfo.depthCompareOp   = GetCompareOp(dsDescriptor.depthCompare);

        depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.minDepthBounds = 0.0f; // Optional
        depthStencilStateCreateInfo.maxDepthBounds = 1.0f; // Optional

        // TODO realxie
        depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
        depthStencilStateCreateInfo.front = {}; // Optional
        depthStencilStateCreateInfo.back = {}; // Optional
    };

    // TODO realxie
    CreateRenderPass(device);

    // The create info chains in a bunch of things created on the stack here or inside state
    // objects.
    VkGraphicsPipelineCreateInfo createInfo;
    {
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderStages;
        createInfo.pVertexInputState = &vertexInputCreateDescription;
        createInfo.pInputAssemblyState = &inputAssembly;
        createInfo.pTessellationState = nullptr;
        createInfo.pViewportState = &viewport;
        createInfo.pRasterizationState = &rasterization;
        createInfo.pMultisampleState = &multisample;
        createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
        createInfo.pColorBlendState = &colorBlending;
        createInfo.pDynamicState = &dynamic;
        createInfo.layout = RHI_CAST(const VKPipelineLayout*, descriptor.layout)->GetNative();
        createInfo.renderPass = renderPass_->GetNative();
        createInfo.subpass = 0;
        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = -1;
    }

    if (vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

}

VKRenderPipeline::~VKRenderPipeline()
{
    // TODO release vulkan resource

    RHI_SAFE_RELEASE(renderPass_);
}

void VKRenderPipeline::Bind(VkCommandBuffer cmdBuffer)
{
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);
}


void VKRenderPipeline::CreateRenderPass(VKDevice* device)
{
    RenderPassCreateInfo renderPassCreateInfo {
            .attachments = {
                    RHI::AttachmentDescription {
                            .format = RHI::Format::B8G8R8A8_UNORM,
                            .samples = RHI::SampleCountFlagBits::SAMPLE_COUNT_1_BIT,
                            .loadOp = RHI::AttachmentLoadOp::CLEAR,
                            .storeOp = RHI::AttachmentStoreOp::STORE,
                            .stencilLoadOp = RHI::AttachmentLoadOp::CLEAR,
                            .stencilStoreOp = RHI::AttachmentStoreOp::STORE,
                            .initialLayout = RHI::ImageLayout::UNDEFINED,
                            .finalLayout = RHI::ImageLayout::PRESENT_SRC_KHR
                    },
                    RHI::AttachmentDescription {
                            .format = RHI::Format::D24_UNORM_S8_UINT,
                            .samples = RHI::SampleCountFlagBits::SAMPLE_COUNT_1_BIT,
                            .loadOp = RHI::AttachmentLoadOp::CLEAR,
                            .storeOp = RHI::AttachmentStoreOp::STORE,
                            .stencilLoadOp = RHI::AttachmentLoadOp::CLEAR,
                            .stencilStoreOp = RHI::AttachmentStoreOp::STORE,
                            .initialLayout = RHI::ImageLayout::UNDEFINED,
                            .finalLayout = RHI::ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                    },
            },
            .subpasses = {
                    SubpassDescription {
                            .colorAttachments = {
                                    RHI::AttachmentReference {
                                            .attachment = 0,
                                            .layout = RHI::ImageLayout::COLOR_ATTACHMENT_OPTIMAL
                                    },
                            },
                            .depthStencilAttachment = {
                                    RHI::AttachmentReference {
                                            .attachment = 1,
                                            .layout = RHI::ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                    },
                            },
                            .inputAttachments = {},
                            .pipelineBindPoint = RHI::PipelineBindPoint::GRAPHICS,
                            .preserveAttachments = {},
                            .resolveAttachments = {}
                    },
            },
            .dependencies = {}
    };

    renderPass_ = (VKRenderPass*)device->CreateRenderPass(renderPassCreateInfo);
    RHI_SAFE_RETAIN(renderPass_);
}

NS_RHI_END
