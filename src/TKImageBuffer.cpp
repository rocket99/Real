#include "TKImageBuffer.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	ImageBuffer::ImageBuffer():
		m_device(VK_NULL_HANDLE),
		m_image(VK_NULL_HANDLE),
		m_imageMem(VK_NULL_HANDLE),
		m_imageView(VK_NULL_HANDLE){}
	
	ImageBuffer::~ImageBuffer(){}

	void ImageBuffer::destroy(){
		if(m_imageView != VK_NULL_HANDLE){
			vkDestroyImageView(m_device, m_imageView, nullptr);
		}

		if(m_imageMem != VK_NULL_HANDLE){
			vkFreeMemory(m_device, m_imageMem, nullptr);
		}
		
		if(m_image != VK_NULL_HANDLE){
			vkDestroyImage(m_device, m_image, nullptr);
		}
	}
	
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
		vkCreateImage(m_device, &imageInfo, nullptr, &m_image);

		VkMemoryAllocateInfo memAllocInfo = {};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAllocInfo.pNext = nullptr;
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(m_device, m_image, &memReqs);
		memAllocInfo.allocationSize = memReqs.size;
		memAllocInfo.memoryTypeIndex = pDevice->getMemoryTypeIndex(memReqs.memoryTypeBits,
																   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		vkAllocateMemory(m_device, &memAllocInfo, nullptr, &m_imageMem);
		vkBindImageMemory(m_device, m_image, m_imageMem, 0);

		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = nullptr;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = pDevice->depthFormat();
		imageViewInfo.image = m_image;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		imageViewInfo.subresourceRange.levelCount = 1;
		vkCreateImageView(m_device, &imageViewInfo, nullptr, &m_imageView);
		return true;
	}

	
};
