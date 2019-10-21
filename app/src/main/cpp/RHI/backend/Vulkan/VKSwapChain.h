//
// Created by realxie on 2019-10-21.
//

#ifndef ALITA_VKSWAPCHAIN_H
#define ALITA_VKSWAPCHAIN_H

#include "VKDevice.h"
#include "VKTextureView.h"

NS_RHI_BEGIN

class VKSwapChain : public SwapChain
{
public:
    VKSwapChain(VKDevice* device);
    virtual ~VKSwapChain();

public:
    virtual TextureView* GetCurrentTexture() override ;

private:
    VKDevice*                       device_             = nullptr;
    std::vector<VKTextureView*>     swapChainImageViews_;
};

NS_RHI_END


#endif //ALITA_VKSWAPCHAIN_H
