//
// Created by realxie on 2019-10-18.
//

#include "VKRenderPassEncoder.h"
#include "VKTypes.h"
#include "VKGraphicPipeline.h"
#include "VKBuffer.h"
#include "VKTextureView.h"
#include "VKBindGroup.h"
#include "VKRenderPass.h"

#include <vector>
#include <array>
#include <vulkan/vulkan.h>

NS_RHI_BEGIN

VKRenderPassEncoder::VKRenderPassEncoder(VKDevice* device)
{
    vkDevice_ = device->GetDevice();

    RenderPassCreateInfo renderPassCreateInfo {
            .attachments = {
                    RHI::AttachmentDescription {
                            .format = RHI::Format::B8G8R8A8_UNORM,
                            .samples = RHI::SampleCountFlagBits::SAMPLE_COUNT_1_BIT,
                            .loadOp = RHI::AttachmentLoadOp::CLEAR,
                            .storeOp = RHI::AttachmentStoreOp::STORE,
                            .stencilLoadOp = RHI::AttachmentLoadOp::DONT_CARE,
                            .stencilStoreOp = RHI::AttachmentStoreOp::DONT_CARE,
                            .initialLayout = RHI::ImageLayout::UNDEFINED,
                            .finalLayout = RHI::ImageLayout::PRESENT_SRC_KHR
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
                            .colorAttachments = {
                                    AttachmentReference {
                                            .attachment = 0,
                                            .layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL
                                    },
                            },
                            .depthStencilAttachment = {},
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

//    VkAttachmentDescription attachmentDescriptions = {
//            .format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
//            .samples = VK_SAMPLE_COUNT_1_BIT,
//            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
//            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
//            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
//            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
//            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
//            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
//    };
//
//    VkAttachmentReference colourReference = {
//            .attachment = 0,
//            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
//    };
//
//    VkSubpassDescription subpassDescription = {
//            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
//            .flags = 0,
//            .inputAttachmentCount = 0,
//            .pInputAttachments = nullptr,
//            .colorAttachmentCount = 1,
//            .pColorAttachments = &colourReference,
//            .pResolveAttachments = nullptr,
//            .pDepthStencilAttachment = nullptr,
//            .preserveAttachmentCount = 0,
//            .pPreserveAttachments = nullptr
//    };
//
//    VkRenderPassCreateInfo renderPassInfo = {
//            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
//            .pNext = nullptr, .attachmentCount = 1,
//            .pAttachments = &attachmentDescriptions,
//            .subpassCount = 1,
//            .pSubpasses = &subpassDescription,
//            .dependencyCount = 0,
//            .pDependencies = nullptr
//    };
//
//    CALL_VK(vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_));
}

VKRenderPassEncoder::~VKRenderPassEncoder()
{
    RHI_SAFE_RELEASE(renderPass_);

    if (vkFramebuffer_)
    {
        vkDestroyFramebuffer(vkDevice_, vkFramebuffer_, nullptr);
        vkFramebuffer_ = 0;
    }
}

void VKRenderPassEncoder::BeginPass(VkCommandBuffer vkCommandBuffer, const RenderPassDescriptor& descriptor)
{
    RHI_ASSERT(vkCommandBuffer != 0L);
    vkCommandBuffer_ = vkCommandBuffer;

    VkCommandBufferBeginInfo cmdBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
    };
    CALL_VK(vkBeginCommandBuffer(vkCommandBuffer_, &cmdBufferBeginInfo));

    // Create framebuffer
    if (vkFramebuffer_)
    {
        vkDestroyFramebuffer(vkDevice_, vkFramebuffer_, nullptr);
        vkFramebuffer_ = 0;
    }

    VKTextureView* colorAttachment = RHI_CAST(VKTextureView*, descriptor.colorAttachments[0].attachment);
    VkImageView attachments[] = {colorAttachment->GetNative()};
    const Extent3D& textureSize = colorAttachment->GetTextureSize();
    VkFramebufferCreateInfo framebufferInfo {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = renderPass_->GetNative(),
            .layers = 1,
            .attachmentCount = 1,  // 2 if using depth
            .pAttachments = attachments,
            .width = textureSize.width,
            .height = textureSize.height,
    };

    CALL_VK(vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &vkFramebuffer_));

    VkClearValue clearVals{
            .color.float32[0] = 1.0f,
            .color.float32[1] = 0.0f,
            .color.float32[2] = 1.0f,
            .color.float32[3] = 1.0f
    };

    VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = renderPass_->GetNative(),
            .framebuffer = vkFramebuffer_,
            .renderArea = {
                    .offset = {.x = 0, .y = 0 },
                    .extent = {.width = textureSize.width, .height = textureSize.height}
            },
            .clearValueCount = 1,
            .pClearValues = &clearVals
    };

    vkCmdBeginRenderPass(vkCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

}

void VKRenderPassEncoder::SetGraphicPipeline(const GraphicPipeline* graphicPipeline)
{
    VkPipeline vkPipeline = RHI_CAST(const VKGraphicPipeline*, graphicPipeline)->GetNative();
    vkCmdBindPipeline(vkCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

    graphicPipeline_ = graphicPipeline;
}

void VKRenderPassEncoder::SetIndexBuffer(const Buffer* buffer, std::uint32_t offset)
{
    VkBuffer vkBuffer = ((VKBuffer*)buffer)->GetNative();
    vkCmdBindIndexBuffer(vkCommandBuffer_, vkBuffer, offset, VK_INDEX_TYPE_UINT16);
}

void VKRenderPassEncoder::SetVertexBuffer(const Buffer* buffer, std::uint32_t offset, std::uint32_t slot)
{
    VkBuffer vkBuffer = ((VKBuffer*)buffer)->GetNative();
    VkBuffer vertexBuffers[] = {vkBuffer};
    VkDeviceSize offsets[] = {offset};
    vkCmdBindVertexBuffers(vkCommandBuffer_, slot, 1, vertexBuffers, offsets);
}

void VKRenderPassEncoder::Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex
        , std::uint32_t firstInstance)
{
    vkCmdDraw(vkCommandBuffer_, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VKRenderPassEncoder::Draw(std::uint32_t vertexCount, std::uint32_t firstVertex)
{
    this->Draw(vertexCount, 1, firstVertex, 0);
}

void VKRenderPassEncoder::DrawIndxed(std::uint32_t indexCount, std::uint32_t firstIndex)
{
    DrawIndexed(indexCount, 1, firstIndex, 0, 0);
}

void VKRenderPassEncoder::DrawIndexed(std::uint32_t indexCount, std::uint32_t instanceCount,
                               std::uint32_t firstIndex, std::uint32_t baseVertex, std::uint32_t firstInstance)
{
    vkCmdDrawIndexed(vkCommandBuffer_, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void VKRenderPassEncoder::SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
{

}

void VKRenderPassEncoder::SetScissorRect(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
{

}

void VKRenderPassEncoder::SetStencilReference(std::uint32_t reference)
{

}

void VKRenderPassEncoder::SetBindGroup(std::uint32_t index, const BindGroup* bindGroup, const std::vector<std::uint32_t>& dynamicOffsets)
{
    RHI_ASSERT(graphicPipeline_);
    auto vkGraphicPipeline = RHI_CAST(const VKGraphicPipeline*, graphicPipeline_);
    RHI_CAST(const VKBindGroup*, bindGroup)->BindToCommandBuffer(index, vkCommandBuffer_, vkGraphicPipeline->GetPipelineLayout());
}

void VKRenderPassEncoder::EndPass()
{
    vkCmdEndRenderPass(vkCommandBuffer_);
    vkCommandBuffer_ = 0L;
    graphicPipeline_ = nullptr;
}

NS_RHI_END