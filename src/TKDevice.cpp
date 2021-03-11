#include "TKDevice.h"
#include <vulkan/vulkan_core.h>

using namespace TK;
Device::Device() {}

Device::~Device() {}

Device *Device::create(const VkInstance &inst, const VkPhysicalDevice &physicalDevice){
	Device *dev = new Device();
	if(!dev->init(inst, physicalDevice)){
		delete dev;
		return nullptr;
	}
	return dev;
}

bool Device::init(const VkInstance &inst, const VkPhysicalDevice &physicalDevice){
	m_instance = inst;
	m_physicalDevice = physicalDevice;
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	m_queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, m_queueFamilyProperties.data());

	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memProperties);
  
	float priorities[] = {0.0f};
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	VkDeviceQueueCreateInfo graphQueueInfo;
	graphQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphQueueInfo.pNext = nullptr;
	graphQueueInfo.flags = 0;
	graphQueueInfo.queueCount = 2;
	graphQueueInfo.pQueuePriorities = priorities;
	graphQueueInfo.queueFamilyIndex = this->getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
	m_graphicsQueueFamilyIndex = graphQueueInfo.queueFamilyIndex;
	queueCreateInfos.push_back(graphQueueInfo);

	VkDeviceQueueCreateInfo compQueueInfo;
	compQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	compQueueInfo.pNext = nullptr;
	compQueueInfo.flags = 0;
	compQueueInfo.pQueuePriorities = priorities;
	compQueueInfo.queueCount = 1;
	compQueueInfo.queueFamilyIndex = this->getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
	m_computeQueueFamilyIndex = compQueueInfo.queueFamilyIndex;
	queueCreateInfos.push_back(compQueueInfo);
	TKLog("graphic queue family index %d,compute queue family index %d",
		  graphQueueInfo.queueFamilyIndex, compQueueInfo.queueFamilyIndex);
	
	std::vector<const char *>enabledLayers =
		{"VK_LAYER_LUNARG_standard_validation",
		 "VK_LAYER_LUNARG_parameter_validation"};
	
	std::vector<const char *>extensionNames =
		{"VK_KHR_swapchain"};
		 //"VK_NV_ray_tracing"};
	VkDeviceCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.enabledLayerCount = enabledLayers.size();
	info.ppEnabledLayerNames = enabledLayers.data();
	info.enabledExtensionCount = extensionNames.size();
	info.ppEnabledExtensionNames = extensionNames.data();
	info.pEnabledFeatures = nullptr;
	info.queueCreateInfoCount = queueCreateInfos.size();
	info.pQueueCreateInfos = queueCreateInfos.data();

	VkResult ret = vkCreateDevice(m_physicalDevice, &info, nullptr, &m_device);
	if(ret != VK_SUCCESS){
		TKError("create device failed %d\n", ret);
		return false;
	}
	TKLog("create device success!");
	vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
	return true;
}

void Device::destroy(){
	if(m_device != VK_NULL_HANDLE){
		vkDestroyDevice(m_device, nullptr);
	}
}

VkDevice Device::logicalDevice() const {
	return m_device;
}

VkQueue Device::graphicsQueue() const {
	return m_graphicsQueue;
}

VkFormat Device::depthFormat() const {
	std::vector<VkFormat> depthFormats = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};
	VkFormat depthFormat;
	for(auto &format : depthFormats){
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &formatProps);
		if(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT){
			depthFormat = format;
			break;
		}
	}
	return depthFormat;
}

uint32_t Device::getQueueFamilyIndex(VkQueueFlagBits flagBits) {
	uint32_t queueFamilyIndex = UINT32_MAX;
	switch(flagBits){
	case VK_QUEUE_GRAPHICS_BIT:
		for(uint32_t i=0; i<m_queueFamilyProperties.size(); ++i){
			uint32_t ret0 = m_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
			if(ret0 == VK_QUEUE_GRAPHICS_BIT){
				queueFamilyIndex = i;
				break;
			}
		}
		break;
	case VK_QUEUE_COMPUTE_BIT:
		for(uint32_t i=0; i<m_queueFamilyProperties.size(); ++i){
			uint32_t ret0 = m_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
			uint32_t ret1 = m_queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT;
       
			if(ret1 == VK_QUEUE_COMPUTE_BIT){// && ret0 != VK_QUEUE_GRAPHICS_BIT){
				queueFamilyIndex = i;
				break;
			}
		}
		break;
	default:
		for(uint32_t i=0; i<m_queueFamilyProperties.size(); ++i){
			uint32_t ret = m_queueFamilyProperties[i].queueFlags & flagBits;
			if(ret == flagBits){
				queueFamilyIndex = i;
				break;
			}
		}
		break;
	};
	return queueFamilyIndex;
}

VkCommandPool Device::createCommandPool(uint32_t queueFamilyIndex) {
	VkCommandPool pool = VK_NULL_HANDLE;
	VkCommandPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolCreateInfo.pNext = nullptr;
	poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	vkCreateCommandPool(m_device, &poolCreateInfo, nullptr, &pool);
	return pool;
}

VkCommandBuffer Device::createCommandBuffer(const VkCommandPool &pool) {
	VkCommandBuffer cmdBuffer;

	VkCommandBufferAllocateInfo cmdBufAllocInfo = {};
	cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocInfo.commandBufferCount = 1;
	cmdBufAllocInfo.commandPool = pool;
	cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkAllocateCommandBuffers(m_device, &cmdBufAllocInfo, &cmdBuffer);
	return cmdBuffer;
}

uint32_t Device::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties){
	uint32_t typeIndex = UINT32_MAX;
	
	for(uint32_t i=0; i<m_memProperties.memoryTypeCount; ++i){
		if((typeBits & 1) == 1){
			if((m_memProperties.memoryTypes[i].propertyFlags & properties) == properties){
				typeIndex = i;
				break;
			}
		}
		typeBits >>= 1;
	}
	return typeIndex;
}

uint32_t Device::graphicsQueueFamilyIndex() const {
	return m_graphicsQueueFamilyIndex;
}

uint32_t Device::computeQueueFamilyIndex() const {
	return m_computeQueueFamilyIndex;
}
