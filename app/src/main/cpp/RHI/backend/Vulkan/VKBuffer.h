//
// Created by realxie on 2019-10-02.
//

#ifndef ALITA_VKBUFFER_H
#define ALITA_VKBUFFER_H

#include "VKDevice.h"

#include "../../include/Macros.h"
#include "../../include/Buffer.h"
#include "../../include/Flags.h"
#include "drivers/vulkan/vulkan_wrapper.h"



#include <vulkan/vulkan.h>

NS_RHI_BEGIN


class VKBuffer : public Buffer
{
public:
    VKBuffer(VKDevice* device, BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data);

    VkBuffer GetNative() const {return vkBuffer_;}

    void UpdateBuffer(const void* data, std::uint32_t offset, std::uint32_t size);

protected:
    virtual ~VKBuffer();

private:
    VkDevice        vkDevice_       = nullptr;
    VkBuffer        vkBuffer_       = 0;
    VkDeviceMemory  vkBufferMemory_ = 0;

};

NS_RHI_END


#endif //ALITA_VKBUFFER_H
