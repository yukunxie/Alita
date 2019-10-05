//
// Created by realxie on 2019-10-04.
//

#ifndef VULKANDEMO_VKTYPES_H
#define VULKANDEMO_VKTYPES_H

#include "../../include/Macros.h"
#include "../../include/Flags.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

VkFormat ToVkFormat(Format format);
VkSampleCountFlagBits ToVkSampleCountFlagBits(SampleCountFlagBits bit);
VkAttachmentLoadOp ToVkAttachmentLoadOp(AttachmentLoadOp op);
VkAttachmentStoreOp ToVkAttachmentStoreOp(AttachmentStoreOp op);
VkImageLayout ToVkImageLayout(ImageLayout layout);
VkShaderStageFlagBits ToVkShaderStageFlagBits(ShaderStageFlagBits bits);
VkVertexInputRate ToVkVertexInputRate(VertexInputRate inputRate);


NS_RHI_END

#endif //VULKANDEMO_VKTYPES_H
