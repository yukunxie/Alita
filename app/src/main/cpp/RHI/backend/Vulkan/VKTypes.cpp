//
// Created by realxie on 2019-10-04.
//

#include "VKTypes.h"

NS_RHI_BEGIN

VkFormat ToVkFormat(Format format)
{
    return (VkFormat)format;
}

VkSampleCountFlagBits ToVkSampleCountFlagBits(SampleCountFlagBits bit)
{
    return (VkSampleCountFlagBits)bit;
}

VkAttachmentLoadOp ToVkAttachmentLoadOp(AttachmentLoadOp op)
{
    return (VkAttachmentLoadOp) op;
}

VkAttachmentStoreOp ToVkAttachmentStoreOp(AttachmentStoreOp op)
{
    return (VkAttachmentStoreOp) op;
}

VkImageLayout ToVkImageLayout(ImageLayout layout)
{
    return (VkImageLayout)layout;
}

VkShaderStageFlagBits ToVkShaderStageFlagBits(ShaderStageFlagBits bits)
{
    return (VkShaderStageFlagBits) bits;
}

VkVertexInputRate ToVkVertexInputRate(VertexInputRate inputRate)
{
    return (VkVertexInputRate)inputRate;
}

VkSharingMode ToVkSharingMode(SharingMode sharingMode)
{
    return (VkSharingMode)sharingMode;
}

VkImageTiling ToVkImageTiling(ImageTiling imageTiling)
{
    return (VkImageTiling)imageTiling;
}

VkImageUsageFlags ToVkImageUsageFlags(ImageUsageFlags imageUsageFlags)
{
    return (VkImageUsageFlags) imageUsageFlags;
}

VkImageType ToVkImageType(ImageType imageType)
{
    return (VkImageType) imageType;
}

VkDescriptorType ToVkDescriptorType(DescriptorType type)
{
    return (VkDescriptorType) type;
}

VkShaderStageFlags ToVkShaderStageFlags(ShaderStageFlags flag)
{
    return (VkShaderStageFlags) flag;
}

VkPrimitiveTopology GetPrimitiveTopology(PrimitiveTopology topology)
{
    return (VkPrimitiveTopology)topology;
}

VkVertexInputRate GetVertexInputRate(InputStepMode mode)
{
    switch (mode)
    {
        case InputStepMode::VERTEX :
            return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
        case InputStepMode::INSTANCE:
            return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
    }
}

std::uint32_t GetVertexFormatSize(VertexFormat format)
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

VkFormat GetVkFormat(VertexFormat format)
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

VkFormat GetVkFormat(TextureFormat format)
{
    switch (format)
    {

        case TextureFormat::R8UNORM :
            return VkFormat::VK_FORMAT_R8_UNORM ;
        case TextureFormat::R8SNORM :
            return VkFormat::VK_FORMAT_R8_SNORM ;
        case TextureFormat::R8UINT :
            return VkFormat::VK_FORMAT_R8_UINT ;
        case TextureFormat::R8SINT :
            return VkFormat::VK_FORMAT_R8_SINT ;

        case TextureFormat::R16UINT :
            return VkFormat::VK_FORMAT_R16_UINT ;
        case TextureFormat::R16SINT :
            return VkFormat::VK_FORMAT_R16_SINT ;
        case TextureFormat::R16FLOAT :
            return VkFormat::VK_FORMAT_R16_SFLOAT ;
        case TextureFormat::RG8UNORM :
            return VkFormat::VK_FORMAT_R8G8_UNORM ;
        case TextureFormat::RG8SNORM :
            return VkFormat::VK_FORMAT_R8G8_SNORM ;
        case TextureFormat::RG8UINT :
            return VkFormat::VK_FORMAT_R8G8_UINT ;
        case TextureFormat::RG8SINT :
            return VkFormat::VK_FORMAT_R8G8_SINT ;

        case TextureFormat::R32UINT :
            return VkFormat::VK_FORMAT_R32_UINT ;
        case TextureFormat::R32SINT :
            return VkFormat::VK_FORMAT_R32_SINT ;
        case TextureFormat::R32FLOAT :
            return VkFormat::VK_FORMAT_R32_SFLOAT ;
        case TextureFormat::RG16UINT :
            return VkFormat::VK_FORMAT_R16G16_UINT ;
        case TextureFormat::RG16SINT :
            return VkFormat::VK_FORMAT_R16G16_SINT ;
        case TextureFormat::RG16FLOAT :
            return VkFormat::VK_FORMAT_R16G16_SFLOAT ;
        case TextureFormat::RGBA8UNORM :
            return VkFormat::VK_FORMAT_R8G8B8A8_UNORM ;
        case TextureFormat::RGBA8UNORM_SRGB :
            return VkFormat::VK_FORMAT_R8G8B8A8_SRGB ;
        case TextureFormat::RGBA8SNORM :
            return VkFormat::VK_FORMAT_R8G8B8A8_SNORM ;
        case TextureFormat::RGBA8UINT :
            return VkFormat::VK_FORMAT_R8G8B8A8_UINT ;
        case TextureFormat::RGBA8SINT :
            return VkFormat::VK_FORMAT_R8G8B8A8_SINT ;
        case TextureFormat::BGRA8UNORM :
            return VkFormat::VK_FORMAT_B8G8R8A8_UNORM ;
        case TextureFormat::BGRA8UNORM_SRGB :
            return VkFormat::VK_FORMAT_B8G8R8A8_SRGB ;

        case TextureFormat::RGB10A2UNORM :
            return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32 ;
        case TextureFormat::RG11B10FLOAT :
            return VkFormat::VK_FORMAT_B10G11R11_UFLOAT_PACK32 ;

        case TextureFormat::RG32UINT :
            return VkFormat::VK_FORMAT_R32G32_UINT ;
        case TextureFormat::RG32SINT :
            return VkFormat::VK_FORMAT_R32G32_SINT ;
        case TextureFormat::RG32FLOAT :
            return VkFormat::VK_FORMAT_R32G32_SFLOAT ;
        case TextureFormat::RGBA16UINT :
            return VkFormat::VK_FORMAT_R16G16B16A16_UINT;
        case TextureFormat::RGBA16SINT :
            return VkFormat::VK_FORMAT_R16G16B16A16_SINT ;
        case TextureFormat::RGBA16FLOAT :
            return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT ;

        case TextureFormat::RGBA32UINT :
            return VkFormat::VK_FORMAT_R32G32B32A32_UINT ;
        case TextureFormat::RGBA32SINT :
            return VkFormat::VK_FORMAT_R32G32B32A32_SINT ;
        case TextureFormat::RGBA32FLOAT :
            return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT ;

        case TextureFormat::DEPTH32FLOAT :
            return VkFormat::VK_FORMAT_D32_SFLOAT ;
        case TextureFormat::DEPTH24PLUS :
            RHI_ASSERT(false);
        case TextureFormat::DEPTH24PLUS_STENCIL8 :
            return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT ;
    }
}

TextureFormat GetTextureFormat(VkFormat format)
{
    switch (format)
    {
        case VkFormat::VK_FORMAT_R8_UNORM:
            return TextureFormat::R8UNORM;
        case VkFormat::VK_FORMAT_R8_SNORM:
            return TextureFormat::R8SNORM;
        case VkFormat::VK_FORMAT_R8_UINT:
            return TextureFormat::R8UINT;
        case VkFormat::VK_FORMAT_R8_SINT:
            return TextureFormat::R8SINT;

        case VkFormat::VK_FORMAT_R16_UINT:
            return TextureFormat::R16UINT;
        case VkFormat::VK_FORMAT_R16_SINT:
            return TextureFormat::R16SINT;
        case VkFormat::VK_FORMAT_R16_SFLOAT:
            return TextureFormat::R16FLOAT;
        case VkFormat::VK_FORMAT_R8G8_UNORM:
            return TextureFormat::RG8UNORM;
        case VkFormat::VK_FORMAT_R8G8_SNORM:
            return TextureFormat::RG8SNORM;
        case VkFormat::VK_FORMAT_R8G8_UINT:
            return TextureFormat::RG8UINT;
        case VkFormat::VK_FORMAT_R8G8_SINT:
            return TextureFormat::RG8SINT;

        case VkFormat::VK_FORMAT_R32_UINT:
            return TextureFormat::R32UINT;
        case VkFormat::VK_FORMAT_R32_SINT:
            return TextureFormat::R32SINT;
        case VkFormat::VK_FORMAT_R32_SFLOAT:
            return TextureFormat::R32FLOAT;
        case VkFormat::VK_FORMAT_R16G16_UINT:
            return TextureFormat::RG16UINT;
        case VkFormat::VK_FORMAT_R16G16_SINT:
            return TextureFormat::RG16SINT;
        case VkFormat::VK_FORMAT_R16G16_SFLOAT:
            return TextureFormat::RG16FLOAT;
        case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
            return TextureFormat::RGBA8UNORM;
        case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
            return TextureFormat::RGBA8UNORM_SRGB;
        case VkFormat::VK_FORMAT_R8G8B8A8_SNORM:
            return TextureFormat::RGBA8SNORM;
        case VkFormat::VK_FORMAT_R8G8B8A8_UINT:
            return TextureFormat::RGBA8UINT;
        case VkFormat::VK_FORMAT_R8G8B8A8_SINT:
            return TextureFormat::RGBA8SINT;
        case VkFormat::VK_FORMAT_B8G8R8A8_UNORM:
            return TextureFormat::BGRA8UNORM;
        case VkFormat::VK_FORMAT_B8G8R8A8_SRGB:
            return TextureFormat::BGRA8UNORM_SRGB;

        case VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return TextureFormat::RGB10A2UNORM;
        case VkFormat::VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return TextureFormat::RG11B10FLOAT;

        case VkFormat::VK_FORMAT_R32G32_UINT:
            return TextureFormat::RG32UINT;
        case VkFormat::VK_FORMAT_R32G32_SINT:
            return TextureFormat::RG32SINT;
        case VkFormat::VK_FORMAT_R32G32_SFLOAT:
            return TextureFormat::RG32FLOAT;
        case VkFormat::VK_FORMAT_R16G16B16A16_UINT:
            return TextureFormat::RGBA16UINT;
        case VkFormat::VK_FORMAT_R16G16B16A16_SINT:
            return TextureFormat::RGBA16SINT;
        case VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT:
            return TextureFormat::RGBA16FLOAT;

        case VkFormat::VK_FORMAT_R32G32B32A32_UINT:
            return TextureFormat::RGBA32UINT;
        case VkFormat::VK_FORMAT_R32G32B32A32_SINT:
            return TextureFormat::RGBA32SINT;
        case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT:
            return TextureFormat::RGBA32FLOAT;

        case VkFormat::VK_FORMAT_D32_SFLOAT:
            return TextureFormat::DEPTH32FLOAT;
        case VkFormat::VK_FORMAT_D24_UNORM_S8_UINT:
            return TextureFormat::DEPTH24PLUS_STENCIL8;
    }
}

VkBool32 GetVkBoolean(bool b)
{
    return b ? VK_TRUE : VK_FALSE;
}

VkCompareOp GetCompareOp(CompareFunction compareFunction)
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

VkAttachmentLoadOp GetLoadOp(LoadOp op)
{
    switch (op)
    {
        case LoadOp ::LOAD:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
        case LoadOp ::CLEAR:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
        case LoadOp ::UNDEFINED:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
}


NS_RHI_END

