#include "TKSwapchain.h"

using namespace TK;
Swapchain::Swapchain(){
	m_swapchain = VK_NULL_HANDLE;
}

Swapchain::~Swapchain(){

}

Swapchain *Swapchain::create(const VkPhysicalDevice &device, const VkSurfaceKHR &surface){
	Swapchain *swapchain = new Swapchain();
	if(!swapchain->init(device, surface)){
		delete swapchain;
		return nullptr;
	}
	return swapchain;
}

bool Swapchain::init(const VkPhysicalDevice &device, const VkSurfaceKHR &surface){
	m_physicalDevice = device;
	m_surface = surface;

	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueProps(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());
	std::vector<VkBool32> supported(queueCount);
	for(uint32_t i=0; i<queueCount; ++i){
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &supported[i]);
	}
	uint32_t graphicQueueIndex = UINT32_MAX;
	uint32_t presentQueueIndex = UINT32_MAX;
	for(uint32_t i=0; i<queueCount; ++i){
		if((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0){
			if(graphicQueueIndex == UINT32_MAX){
				graphicQueueIndex = i;
			}
			if(supported[i] == VK_TRUE){
				graphicQueueIndex = i;
				presentQueueIndex = i;
				break;
			}
		}

	}
	if(presentQueueIndex == UINT32_MAX){
		for(uint32_t i=0; i<queueCount; ++i){
			if(supported[i] == VK_TRUE){
				presentQueueIndex = i;
				break;
			}
		}
	}
	if(graphicQueueIndex == UINT32_MAX || presentQueueIndex == UINT32_MAX){
		return false;
	}
	m_queueNodeIndex = graphicQueueIndex;

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, surfaceFormats.data());
	if((formatCount==1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)){
		m_colorSpace = surfaceFormats[0].colorSpace;
		m_colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
	}else{
		bool found = false;
		for(auto && surfaceFormat : surfaceFormats){
			if(surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM){
				m_colorFormat = surfaceFormat.format;
				m_colorSpace = surfaceFormat.colorSpace;
				found = true;
				break;
			}
		}
		if(found == false){
			m_colorSpace = surfaceFormats[0].colorSpace;
			m_colorFormat = surfaceFormats[0].format;
		}
	}
	
	return true;
}

void Swapchain::setupSwapchain(uint32_t *width, uint32_t *height, const VkDevice &device){
	m_device = device;
	if(m_device == VK_NULL_HANDLE){
		printf("device create swapchain!\n");
		return;
	}
	VkSwapchainKHR oldSwapchain = m_swapchain;
	
	VkSurfaceCapabilitiesKHR surfCaps;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfCaps);
	uint32_t presentMondeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentMondeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentMondeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentMondeCount, presentModes.data());

	VkExtent2D swapchainExtent = {};
	if (surfCaps.currentExtent.width == (uint32_t)-1) {
		swapchainExtent.width = *width;
		swapchainExtent.height = *height;
	} else {
		swapchainExtent = surfCaps.currentExtent;
		*width = surfCaps.currentExtent.width;
		*height = surfCaps.currentExtent.height;
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

	for(uint32_t i=0; i<presentMondeCount; ++i){
		if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR){
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		if((presentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
		   (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)){
			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	uint32_t desiredSwapchainImageCount = surfCaps.minImageCount+1;
	if((surfCaps.maxImageCount > 0) &&
	   (desiredSwapchainImageCount > surfCaps.maxImageCount)){
		desiredSwapchainImageCount = surfCaps.maxImageCount;
	}
	VkSurfaceTransformFlagsKHR preTransform;
	if(surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR){
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}else{
		preTransform = surfCaps.currentTransform;
	}
	printf("swap image count %d\n", desiredSwapchainImageCount);
	
	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {};
	compositeAlphaFlags.push_back(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
    compositeAlphaFlags.push_back(VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR);
    compositeAlphaFlags.push_back(VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR);
    compositeAlphaFlags.push_back(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR);
 
	for(auto & compositeAlphaFlag : compositeAlphaFlags){
		if(surfCaps.supportedCompositeAlpha & compositeAlphaFlag){
			compositeAlpha = compositeAlphaFlag;
			break;
		}
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = m_surface;
	swapchainCreateInfo.minImageCount = desiredSwapchainImageCount;
	swapchainCreateInfo.imageFormat = m_colorFormat;
	swapchainCreateInfo.imageColorSpace = m_colorSpace;
	swapchainCreateInfo.imageExtent = {swapchainExtent.width, swapchainExtent.height};
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.compositeAlpha = compositeAlpha;
	if(surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT){
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}
	if(surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT){
		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	VkResult result = vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain);
	if(result != VK_SUCCESS){
		printf("create swapchain failed, error %d", result);
	}
	
	if(oldSwapchain != VK_NULL_HANDLE){
		for(uint32_t i=0; i<m_imageCount; ++i){
			vkDestroyImageView(m_device, m_buffers[i].view, nullptr);
		}
		vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
	}

	vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, nullptr);
	m_images.resize(m_imageCount);
	vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, m_images.data());
	m_buffers.resize(m_imageCount);
	for(uint32_t i=0; i<m_imageCount; ++i){
		VkImageViewCreateInfo colorAttachViewInfo = {};
		colorAttachViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		colorAttachViewInfo.pNext = nullptr;
		colorAttachViewInfo.format = m_colorFormat;
		colorAttachViewInfo.components =
			{
			 VK_COMPONENT_SWIZZLE_R,
			 VK_COMPONENT_SWIZZLE_G,
			 VK_COMPONENT_SWIZZLE_B,
			 VK_COMPONENT_SWIZZLE_A
			};
		colorAttachViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorAttachViewInfo.subresourceRange.baseMipLevel = 0;
		colorAttachViewInfo.subresourceRange.levelCount = 1;
		colorAttachViewInfo.subresourceRange.baseArrayLayer = 0;
		colorAttachViewInfo.subresourceRange.layerCount = 1;
		colorAttachViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorAttachViewInfo.flags = 0;

		m_buffers[i].image = m_images[i];
		colorAttachViewInfo.image = m_buffers[i].image;
		vkCreateImageView(m_device, &colorAttachViewInfo, nullptr, &m_buffers[i].view);
	}
}

void Swapchain::clean(){
	if(m_swapchain != VK_NULL_HANDLE){
		for(uint32_t  i=0; i<m_imageCount; ++i){
			vkDestroyImageView(m_device, m_buffers[i].view, nullptr);
		}
	}
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	m_swapchain = VK_NULL_HANDLE;
}

VkFormat Swapchain::colorFormat() const {
	return m_colorFormat;
}

uint32_t Swapchain::queueFamilyIndex() const {
	return m_queueNodeIndex;
}

uint32_t Swapchain::imageCount() const {
	return m_imageCount;
}

VkImageView Swapchain::imageView(uint32_t idx) const {
	return m_buffers[idx].view;
}

VkSwapchainKHR Swapchain::swapchain() const {
	return m_swapchain;
}


