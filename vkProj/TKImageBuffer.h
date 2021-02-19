//
//TKImageBuffer.h
//

#ifndef __TKImageBuffer_h__
#define __TKImageBuffer_h__

#include "TKDevice.h"
#include "TKPrefix.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	class ImageBuffer{
	public:
		ImageBuffer();
		~ImageBuffer();
		static ImageBuffer *createDepthImageBuffer(Device *device, uint32_t width, uint32_t height);
		bool initDepthImageBuffer(Device *device, uint32_t width, uint32_t height);
	private:
          VkDevice m_device;
		  VkImage m_vkImage;
          uint32_t m_width, m_height;
	};
};


#endif //__TKImageBuffer_h__
