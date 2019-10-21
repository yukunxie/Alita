//
// Created by realxie on 2019-10-21.
//

#ifndef ALITA_DESCRIPTORS_H
#define ALITA_DESCRIPTORS_H

#include "Macros.h"
#include "Flags.h"
#include "TextureView.h"

#include <vector>

NS_RHI_BEGIN

struct RenderPassColorAttachmentDescriptor
{
    TextureView* attachment;
    TextureView* resolveTarget;
    // loadValue;
    AttachmentLoadOp loadOp;
    AttachmentStoreOp storeOp;
};

struct RenderPassDepthStencilAttachmentDescriptor
{
    TextureView* attachment;

    AttachmentLoadOp depthLoadOp;
    AttachmentStoreOp depthStoreOp;
    float depthLoadValue = 1.0f;

    AttachmentLoadOp stencilLoadOp;
    AttachmentStoreOp stencilStoreOp;
    std::uint32_t stencilLoadValue = 0;
};

struct ObjectDescriptorBase {
    std::string label;
};

struct RenderPassDescriptor : ObjectDescriptorBase {
    std::vector<RenderPassColorAttachmentDescriptor> colorAttachments;
    RenderPassDepthStencilAttachmentDescriptor depthStencilAttachment;
};

NS_RHI_END

#endif //ALITA_DESCRIPTORS_H
