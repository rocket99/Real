//
//TKSwapchain.h
//
#ifndef __TKSwapchain_h__
#define __TKSwapchain_h__

#include "TKPrefix.h"

//#include "TKDevice.h"

namespace TK {
	class Swapchain {
	public:
		Swapchain();
		~Swapchain();
		static Swapchain*create(const VkPhysicalDevice &device, const VkSurfaceKHR &surface);
		bool init(const VkPhysicalDevice &device, const VkSurfaceKHR &surface);

		void setupSwapchain(uint32_t *width, uint32_t *height, const VkDevice &device);
		
		void clean();
		VkFormat colorFormat() const;
		uint32_t queueFamilyIndex() const;
		uint32_t imageCount() const;
		VkImageView imageView(uint32_t idx) const;
		VkSwapchainKHR swapchain() const;
	private:
		VkDevice m_device;
		VkPhysicalDevice m_physicalDevice;
		VkSurfaceKHR m_surface;
		VkSwapchainKHR m_swapchain;

		VkSwapchainCreateInfoKHR m_createInfo;

		uint32_t m_queueNodeIndex;
		VkColorSpaceKHR m_colorSpace;
		VkFormat m_colorFormat;

		uint32_t m_imageCount;
		std::vector<VkImage> m_images;
		struct buffer {
			VkImage image;
			VkImageView view;
		};
		std::vector<buffer> m_buffers;
	};
};
#endif // TKSwapchain.h





