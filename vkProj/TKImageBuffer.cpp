#include "TKImageBuffer.h"

namespace TK {

	ImageBuffer::ImageBuffer(){}
	ImageBuffer::~ImageBuffer(){}

	ImageBuffer *ImageBuffer::createDepthImageBuffer(Device *device, uint32_t width, uint32_t height){
		ImageBuffer *imgBuf = new ImageBuffer();
		if(imgBuf->initDepthImageBuffer(device, width, height)==false){
			delete imgBuf;
			return nullptr;
		}
		return imgBuf;
	}

	bool ImageBuffer::initDepthImageBuffer(Device *pDevice, uint32_t width, uint32_t height){
		m_width = width;
		m_height = height;
		m_device = pDevice->logicalDevice();

		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.format = pDevice->depthFormat();
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.extent = {m_width, m_height, 1};
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.flags = 0;
		vkCreateImage(m_device, &imageInfo, nullptr, &m_vkImage);

		VkMemoryAllocateInfo memAllocInfo = {};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.pNext = nullptr;
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(m_device, m_vkImage, &memReqs);
		memAllocInfo.allocationSize = memReqs.size;
		memAllocInfo.memoryTypeIndex = 
			pDevice->getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		
		return true;
	}

	
};
