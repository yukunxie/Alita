//
// Created by realxie on 2019-10-21.
//

#ifndef ALITA_DESCRIPTORS_H
#define ALITA_DESCRIPTORS_H

#include "Macros.h"
#include "Flags.h"

#include <vector>
#include <optional>

NS_RHI_BEGIN

class TextureView;
class Texture;
class PipelineLayout;

enum class TextureFormat {
    // 8-bit formats
    R8UNORM,
    R8SNORM,
    R8UINT,
    R8SINT,

    // 16-bit formats
    R16UINT,
    R16SINT,
    R16FLOAT,
    RG8UNORM,
    RG8SNORM,
    RG8UINT,
    RG8SINT,

    // 32-bit formats
    R32UINT,
    R32SINT,
    R32FLOAT,
    RG16UINT,
    RG16SINT,
    RG16FLOAT,
    RGBA8UNORM,
    RGBA8UNORM_SRGB,
    RGBA8SNORM,
    RGBA8UINT,
    RGBA8SINT,
    BGRA8UNORM,
    BGRA8UNORM_SRGB,

    // Packed 32-bit formats
    RGB10A2UNORM,
    RG11B10FLOAT,

    // 64-bit formats
    RG32UINT,
    RG32SINT,
    RG32FLOAT,
    RGBA16UINT,
    RGBA16SINT,
    RGBA16FLOAT,

    // 128-bit formats
    RGBA32UINT,
    RGBA32SINT,
    RGBA32FLOAT,

    // Depth and stencil formats
    DEPTH32FLOAT,
    DEPTH24PLUS,
    DEPTH24PLUS_STENCIL8
};

enum class ColorWrite {
    RED   = 0x1,
    GREEN = 0x2,
    BLUE  = 0x4,
    ALPHA = 0x8,
    ALL   = 0xF,
};

enum class VertexFormat {
    UCHAR2,
    UCHAR4,
    CHAR2,
    CHAR4,
    UCHAR2NORM,
    UCHAR4NORM,
    CHAR2NORM,
    CHAR4NORM,
    USHORT2,
    USHORT4,
    SHORT2,
    SHORT4,
    USHORT2NORM,
    USHORT4NORM,
    SHORT2NORM,
    SHORT4NORM,
    HALF2,
    HALF4,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    UINT,
    UINT2,
    UINT3,
    UINT4,
    INT,
    INT2,
    INT3,
    INT4
};

enum class IndexFormat {
    UINT16,
    UINT32,
};

enum class LoadOp {
    LOAD,
    CLEAR,
    UNDEFINED,
};

enum class StoreOp {
    STORE,
    CLEAR,
};

union Color {
    struct {
        float r;
        float g;
        float b;
        float a;
    };

    struct {
        float x;
        float y;
        float z;
        float w;
    };
};

struct RenderPassColorAttachmentDescriptor
{
    TextureView* attachment = nullptr;
    TextureView* resolveTarget = nullptr;
    Color loadValue;
    LoadOp loadOp;
    StoreOp storeOp;
};

struct RenderPassDepthStencilAttachmentDescriptor
{
    TextureView* attachment = nullptr;

    LoadOp depthLoadOp;
    StoreOp depthStoreOp;
    float depthLoadValue = 1.0f;

    LoadOp stencilLoadOp;
    StoreOp stencilStoreOp;
    std::uint32_t stencilLoadValue = 0;
};

struct ObjectDescriptorBase {
    std::string label;
};

struct RenderPassDescriptor : ObjectDescriptorBase {
    std::vector<RenderPassColorAttachmentDescriptor>    colorAttachments;
    RenderPassDepthStencilAttachmentDescriptor          depthStencilAttachment;
};

struct ProgrammableStageDescriptor {
    ShaderModule*   module;
    std::string     entryPoint;
    // TODO: other stuff like specialization constants?
};

struct RasterizationStateDescriptor {
    FrontFace       frontFace           = FrontFace::COUNTER_CLOCKWISE;
    CullMode        cullMode            = CullMode::NONE;
    std::uint32_t   depthBias           = 0;
    float           depthBiasSlopeScale = 0;
    float           depthBiasClamp      = 0;
};

enum class CompareFunction {
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS
};

struct BlendDescriptor {
    BlendFactor srcFactor = BlendFactor::ONE;
    BlendFactor dstFactor = BlendFactor::ZERO;
    BlendOp     operation = BlendOp::ADD;
};

struct ColorStateDescriptor {
    TextureFormat   format;
    BlendDescriptor alphaBlend;
    BlendDescriptor colorBlend;
    ColorWrite      writeMask = ColorWrite::ALL ;  // GPUColorWrite.ALL
};

enum class StencilOperation {
    KEEP,
    ZERO,
    REPLACE,
    INVERT,
    INCREMENT_CLAMP,
    DECREMENT_CLAMP,
    INCREMENT_WRAP,
    DECREMENT_WRAP
};

struct StencilStateFaceDescriptor {
    CompareFunction compare         = CompareFunction::ALWAYS;
    StencilOperation failOp         = StencilOperation::KEEP;
    StencilOperation depthFailOp    = StencilOperation::KEEP;
    StencilOperation passOp         = StencilOperation::KEEP;
};

struct DepthStencilStateDescriptor {
    TextureFormat format;

    bool depthWriteEnabled = false;
    CompareFunction depthCompare = CompareFunction::ALWAYS;

    StencilStateFaceDescriptor stencilFront;
    StencilStateFaceDescriptor stencilBack;

    std::uint32_t stencilReadMask   = 0xFFFFFFFF;
    std::uint32_t stencilWriteMask  = 0xFFFFFFFF;
};

struct VertexAttributeDescriptor {
    BufferSize      offset = 0;
    VertexFormat    format;
    std::uint32_t   shaderLocation;
};

enum class InputStepMode {
    VERTEX,
    INSTANCE
};

struct VertexBufferDescriptor {
    BufferSize stride;
    InputStepMode stepMode = InputStepMode::VERTEX;
    std::vector<VertexAttributeDescriptor> attributeSet;
};

struct VertexInputDescriptor {
    IndexFormat indexFormat = IndexFormat ::UINT32;
    std::vector<VertexBufferDescriptor> vertexBuffers;
};

struct PipelineDescriptorBase : ObjectDescriptorBase {
    PipelineLayout* layout;
};

struct RenderPipelineDescriptor : public PipelineDescriptorBase {
    ProgrammableStageDescriptor vertexStage;
    ProgrammableStageDescriptor fragmentStage;

    PrimitiveTopology primitiveTopology;
    RasterizationStateDescriptor rasterizationState;
    std::vector<ColorStateDescriptor> colorStates;
    std::optional<DepthStencilStateDescriptor> depthStencilState;
    VertexInputDescriptor vertexInput;

    std::uint32_t sampleCount = 1;
    std::uint32_t sampleMask = 0xFFFFFFFF;
    bool          alphaToCoverageEnabled = false;
    // TODO: other properties
};

enum class TextureDimension {
    TEXTURE_1D,
    TEXTURE_2D,
    TEXTURE_3D
};

enum class TextureUsage {
    COPY_SRC          = 0x01,
    COPY_DST          = 0x02,
    SAMPLED           = 0x04,
    STORAGE           = 0x08,
    OUTPUT_ATTACHMENT = 0x10,
};

typedef std::uint32_t TextureUsageFlags;

struct TextureDescriptor : public ObjectDescriptorBase {
    Extent3D size;
    std::uint32_t arrayLayerCount = 1;
    std::uint32_t mipLevelCount = 1;
    std::uint32_t sampleCount = 1;
    TextureDimension dimension = TextureDimension::TEXTURE_2D;
    TextureFormat format;
    TextureUsageFlags usage;
};

enum BufferUsage {
    MAP_READ  = 0x0001,
    MAP_WRITE = 0x0002,
    COPY_SRC  = 0x0004,
    COPY_DST  = 0x0008,
    INDEX     = 0x0010,
    VERTEX    = 0x0020,
    UNIFORM   = 0x0040,
    STORAGE   = 0x0080,
    INDIRECT  = 0x0100,
};

typedef std::uint32_t BufferUsageFlags;

struct BufferDescriptor : ObjectDescriptorBase {
    BufferSize size;
    BufferUsageFlags usage;
};

NS_RHI_END

#endif //ALITA_DESCRIPTORS_H
