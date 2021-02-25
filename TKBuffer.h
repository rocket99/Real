//
//TKBuffer.h
//

#ifndef __TKBuffer_h__
#define __TKBuffer_h__

#include "TKDevice.h"

namespace TK{
	class Buffer{
	public:
		Buffer();
		~Buffer();

		static Buffer* create(Device *device, VkBufferUsageFlags usageFlags, VkDeviceSize size, VkSharingMode shareMode);
		bool init(Device *device, VkBufferUsageFlags usageFlags, VkDeviceSize size, VkSharingMode shareMode);


	protected:
		Device *pDevice;
		VkBuffer m_vkBuf;
		VkDeviceMemory m_memory;
		VkBufferCreateInfo m_createInfo;
	};

};

#endif //__TKBuffer_h__
