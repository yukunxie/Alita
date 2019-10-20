//
// Created by realxie on 2019-10-18.
//

#include "VKRenderPassEncoder.h"
#include "VKTypes.h"
#include "VKGraphicPipeline.h"
#include "VKBuffer.h"

#include <vector>
#include <array>
#include <vulkan/vulkan.h>

NS_RHI_BEGIN


VKRenderPassEncoder::~VKRenderPassEncoder()
{

}

void VKRenderPassEncoder::BeginPass(VkCommandBuffer vkCommandBuffer)
{
    RHI_ASSERT(vkCommandBuffer != 0L);
    vkCommandBuffer_ = vkCommandBuffer;
}

void VKRenderPassEncoder::SetGraphicPipeline(const GraphicPipeline* graphicPipeline)
{
    VkPipeline vkPipeline = RHI_CAST(const VKGraphicPipeline*, graphicPipeline)->GetNative();
    vkCmdBindPipeline(vkCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
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

void VKRenderPassEncoder::EndPass()
{
    vkCommandBuffer_ = 0L;
}

NS_RHI_END