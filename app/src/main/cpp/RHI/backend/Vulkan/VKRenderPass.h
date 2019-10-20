//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_VKRENDERPASS_H
#define ALITA_VKRENDERPASS_H

#include "VKDevice.h"
#include "../../include/Macros.h"
#include "../../include/RenderPass.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

class VKRenderPass : public RenderPass
{
public:
    VKRenderPass(VKDevice* device, const RenderPassCreateInfo& createInfo);
    virtual ~VKRenderPass();

    VkRenderPass GetNative() {return vkRenderPass_;}

protected:
    void ParseAttachmentDescriptions(const RenderPassCreateInfo& createInfo, std::vector<VkAttachmentDescription>& descriptions);
    void ParseSubpassDescriptions(const RenderPassCreateInfo& createInfo, std::vector<VkSubpassDescription>& descriptions, std::vector<std::vector<VkAttachmentReference>>& references);
    void ParseAttachmentReferences(const std::vector<AttachmentReference>& attachments, std::vector<VkAttachmentReference>& attachmentReferences);

private:
    VkRenderPass   vkRenderPass_        = 0L;
    VkCommandBuffer vkCommandBuffer_    = 0L;
};

NS_RHI_END


#endif //ALITA_VKRENDERPASS_H
