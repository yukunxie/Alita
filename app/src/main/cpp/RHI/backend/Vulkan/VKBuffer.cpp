//
// Created by realxie on 2019-10-02.
//

#include "VKBuffer.h"

NS_RHI_BEGIN

VKBuffer::VKBuffer(VKDevice* device, BufferUsageFlagBits usageFlagBits, SharingMode sharingMode, std::uint32_t sizeOfBytes, const void* data)
{
    vkDevice_ = device->GetVulkanDevice();

    // TODO define a class member
    uint32_t queueFamilyIndex_ = 0;

    VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = sizeOfBytes,
            .usage = (VkBufferUsageFlagBits)usageFlagBits,
            .sharingMode = (VkSharingMode)sharingMode,
            .pQueueFamilyIndices = &queueFamilyIndex_,
            .queueFamilyIndexCount = 1,
    };

    if (vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &vkBuffer_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    // Allocate heap memory for vertex buffer

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice_, vkBuffer_, &memRequirements);

    std::uint32_t memTypeMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VkMemoryAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, memTypeMask)
    };

    CALL_VK(vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &vkBufferMemory_));

    void* accessPointer;
    CALL_VK(vkMapMemory(vkDevice_, vkBufferMemory_, 0, bufferInfo.size, 0, &accessPointer));
    memcpy(accessPointer, data, (size_t) bufferInfo.size);
    vkUnmapMemory(vkDevice_, vkBufferMemory_);
    CALL_VK(vkBindBufferMemory(vkDevice_, vkBuffer_, vkBufferMemory_, 0));
}

void VKBuffer::UpdateBuffer(const void* data, std::uint32_t offset, std::uint32_t size)
{
    void* accessPointer;
    CALL_VK(vkMapMemory(vkDevice_, vkBufferMemory_, offset, size, 0, &accessPointer));
    memcpy(accessPointer, data, (size_t) size);
    vkUnmapMemory(vkDevice_, vkBufferMemory_);
    CALL_VK(vkBindBufferMemory(vkDevice_, vkBuffer_, vkBufferMemory_, 0));
}

NS_RHI_END
