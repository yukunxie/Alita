//
// Created by realxie on 2019-10-04.
//

#include "VKRenderPass.h"
#include "VKTypes.h"

#include <vector>
#include <array>
#include <vulkan/vulkan.h>


NS_RHI_BEGIN

VKRenderPass::VKRenderPass(VKDevice* device, const RenderPassCreateInfo& createInfo)
{
    // Setup descriptions for attachments.
    std::vector<VkAttachmentDescription> attachmentDescriptions(createInfo.attachments.size());
    ParseAttachmentDescriptions(createInfo, attachmentDescriptions);

    // Setup descriptions for subpasses.
    std::vector<VkSubpassDescription> subpassDescriptions(createInfo.subpasses.size());
    std::vector<std::vector<VkAttachmentReference>> attachReferences;
    ParseSubpassDescriptions(createInfo, subpassDescriptions, attachReferences);

    VkRenderPassCreateInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .attachmentCount = (std::uint32_t)attachmentDescriptions.size(),
            .pAttachments = attachmentDescriptions.data(),
            .subpassCount = (std::uint32_t)subpassDescriptions.size(),
            .pSubpasses = subpassDescriptions.data(),
            .dependencyCount = 0,
            .pDependencies = nullptr
    };

    CALL_VK(vkCreateRenderPass(device->GetVulkanDevice(), &renderPassInfo, nullptr, &vkRenderPass_));
}

VKRenderPass::~VKRenderPass()
{
}

void VKRenderPass::ParseAttachmentDescriptions(const RenderPassCreateInfo& createInfo, std::vector<VkAttachmentDescription>& descriptions)
{
    descriptions.resize(createInfo.attachments.size());
    for (size_t i = 0; i < createInfo.attachments.size(); ++i)
    {
        const AttachmentDescription& desc = createInfo.attachments[i];
        VkAttachmentDescription& target = descriptions[i];
        target.format = ToVkFormat(desc.format);
        target.samples = ToVkSampleCountFlagBits(desc.samples);
        target.loadOp = ToVkAttachmentLoadOp(desc.loadOp);
        target.storeOp = ToVkAttachmentStoreOp(desc.storeOp);
        target.stencilLoadOp = ToVkAttachmentLoadOp(desc.stencilLoadOp);
        target.stencilStoreOp = ToVkAttachmentStoreOp(desc.stencilStoreOp);
        target.initialLayout = ToVkImageLayout(desc.initialLayout);
        target.finalLayout = ToVkImageLayout(desc.finalLayout);
    }
}

void VKRenderPass::ParseSubpassDescriptions(const RenderPassCreateInfo& createInfo
        , std::vector<VkSubpassDescription>& descriptions
        , std::vector<std::vector<VkAttachmentReference>>& references
        )
{
    descriptions.resize(createInfo.subpasses.size());
    references.resize(descriptions.size());

    for (size_t i = 0; i < createInfo.subpasses.size(); ++i)
    {
        const SubpassDescription& sp = createInfo.subpasses[i];
        RHI_ASSERT(sp.inputAttachments.size() == 0);
        RHI_ASSERT(sp.resolveAttachments.size() == 0);
        RHI_ASSERT(sp.preserveAttachments.size() == 0);

        VkSubpassDescription& description = descriptions[i];
        description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        description.flags = 0;
        description.inputAttachmentCount = 0;
        description.pInputAttachments = nullptr;

        ParseAttachmentReferences(sp.colorAttachments, references[i]);
        description.colorAttachmentCount = references[i].size();
        description.pColorAttachments = references[i].data();
        description.pResolveAttachments = nullptr;
        description.pDepthStencilAttachment = nullptr;
        description.preserveAttachmentCount = 0;
        description.pPreserveAttachments = nullptr;
    }
}

void VKRenderPass::ParseAttachmentReferences(const std::vector<AttachmentReference>& attachments, std::vector<VkAttachmentReference>& attachmentReferences)
{
    attachmentReferences.resize(attachments.size());
    for (size_t i = 0; i < attachments.size(); ++i)
    {
        attachmentReferences[i].attachment = attachments[i].attachment;
        attachmentReferences[i].layout = ToVkImageLayout(attachments[i].layout);
    }
}

NS_RHI_END