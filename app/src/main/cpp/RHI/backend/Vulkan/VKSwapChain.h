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
    virtual void Present(const Queue* queue) override ;

protected:
    void Init();
    void Dispose();
    void RecreateSwapChain();

private:
    VKDevice*                       device_                     = nullptr;
    VkSemaphore                     vkImageAvailableSemaphore_  = 0L;
    std::vector<VKTextureView*>     swapChainImageViews_;
    std::uint32_t imageIndex_   = 0;
};

NS_RHI_END


#endif //ALITA_VKSWAPCHAIN_H
