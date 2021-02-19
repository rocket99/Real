//
//TKDevice.h
//

#ifndef __TKDevice_h__
#define __TKDevice_h__

#include "TKPrefix.h"

namespace TK {
	class Device {
	public:
		Device();
		~Device();
		static Device *create(const VkInstance &inst, const VkPhysicalDevice &physicalDevice);
		bool init(const VkInstance &inst, const VkPhysicalDevice &physicalDevice);

		VkCommandPool createCommandPool(uint32_t queueFamiliyIndex);
		VkCommandBuffer createCommandBuffer();
		
		VkDevice logicalDevice() const;
		VkFormat depthFormat() const;

		void destroy();

		VkCommandBuffer createCommandBuffer(const VkCommandPool &pool);
		uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);

		uint32_t graphicsQueueFamilyIndex() const;
		uint32_t computeQueueFamilyIndex() const; 
		VkQueue graphicsQueue() const;
	private:
		VkDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkInstance m_instance = VK_NULL_HANDLE;

		VkQueue m_graphicsQueue;
		std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;
		uint32_t getQueueFamilyIndex(VkQueueFlagBits flagBit);

		VkPhysicalDeviceMemoryProperties m_memProperties;
		uint32_t m_graphicsQueueFamilyIndex = UINT32_MAX;
		uint32_t m_computeQueueFamilyIndex = UINT32_MAX;
	};
};

#endif // TKDevice.h


