//
// Created by realxie on 2019-10-04.
//

#ifndef ALITA_VKTYPES_H
#define ALITA_VKTYPES_H

#include "../../include/Macros.h"
#include "../../include/Flags.h"
#include "../../include/Descriptors.h"

#include <vulkan/vulkan.h>

NS_RHI_BEGIN

VkFormat ToVkFormat(Format format);
VkSampleCountFlagBits ToVkSampleCountFlagBits(SampleCountFlagBits bit);
VkAttachmentLoadOp ToVkAttachmentLoadOp(AttachmentLoadOp op);
VkAttachmentStoreOp ToVkAttachmentStoreOp(AttachmentStoreOp op);
VkImageLayout ToVkImageLayout(ImageLayout layout);
VkShaderStageFlagBits ToVkShaderStageFlagBits(ShaderStageFlagBits bits);
VkVertexInputRate ToVkVertexInputRate(VertexInputRate inputRate);
VkSharingMode ToVkSharingMode(SharingMode sharingMode);
VkImageTiling ToVkImageTiling(ImageTiling imageTiling);
VkImageUsageFlags ToVkImageUsageFlags(ImageUsageFlags imageUsageFlags);
VkImageType ToVkImageType(ImageType imageType);
VkDescriptorType ToVkDescriptorType(DescriptorType type);
VkShaderStageFlags ToVkShaderStageFlags(ShaderStageFlags flag);

VkPrimitiveTopology GetPrimitiveTopology(PrimitiveTopology topology);
VkVertexInputRate GetVertexInputRate(InputStepMode mode);
std::uint32_t GetVertexFormatSize(VertexFormat format);
VkFormat GetVkFormat(VertexFormat format);
VkFormat GetVkFormat(TextureFormat format);
TextureFormat GetTextureFormat(VkFormat format);
VkBool32 GetVkBoolean(bool b);
VkCompareOp GetCompareOp(CompareFunction compareFunction);
VkAttachmentLoadOp GetLoadOp(LoadOp op);


NS_RHI_END

#endif //ALITA_VKTYPES_H
