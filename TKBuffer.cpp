#include "TKBuffer.h"
#include <vulkan/vulkan_core.h>

namespace TK{
	Buffer::Buffer():
		m_vkBuf(VK_NULL_HANDLE),
		pDevice(nullptr){
		m_createInfo = {};
	}
	
	Buffer::~Buffer(){}

    Buffer*	Buffer::create(Device *device, VkBufferUsageFlags usageFlags,
						   VkDeviceSize size, VkSharingMode shareMode){
		Buffer *buf = new Buffer();
		if(!buf->init(device, usageFlags, size, shareMode)){
			delete buf;
			return nullptr;
		}
		return buf;
	}

	bool Buffer::init(Device *device, VkBufferUsageFlags usageFlags,
					  VkDeviceSize size, VkSharingMode shareMode){
		pDevice = device;
		m_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		m_createInfo.pNext = nullptr;
		m_createInfo.sharingMode = shareMode;
		m_createInfo.flags = 0;
		m_createInfo.size = size;
		m_createInfo.usage = usageFlags;
		m_createInfo.queueFamilyIndexCount = 0;
		m_createInfo.pQueueFamilyIndices = nullptr;

		if(VK_SUCCESS != vkCreateBuffer(pDevice->logicalDevice(), &m_createInfo, nullptr, &m_vkBuf)){
			return false;
		}
		VkMemoryRequirements memReq;
		vkGetBufferMemoryRequirements(pDevice->logicalDevice(), m_vkBuf, &memReq);
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex =
			pDevice->getMemoryTypeIndex(memReq.memoryTypeBits,
										VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
										VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		
		vkAllocateMemory(pDevice->logicalDevice(), &allocInfo, nullptr, &m_memory);
		vkBindBufferMemory(pDevice->logicalDevice(), m_vkBuf, m_memory, 0);

		return true;
	}
};
