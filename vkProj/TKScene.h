//
//TKScene.h
//

#ifndef __TKScene_h__
#define __TKScene_h__

#include "TKDevice.h"
#include "TKSwapchain.h"
#include "TKRenderPass.h"
#include "TKPipeline.h"

namespace TK {

	class Scene {
	public:
		Scene();
		~Scene();

		static Scene *create(const VkInstance &inst, const VkSurfaceKHR &surface);
		bool init(const VkInstance &inst, const VkSurfaceKHR &surface);

		void setSize(uint32_t w, uint32_t h);
		
		void destroy();

		void draw();
	protected:
		uint32_t m_currentIndex;
		uint32_t m_width, m_height;
		VkInstance m_instance = VK_NULL_HANDLE;
		VkSurfaceKHR  m_surface = VK_NULL_HANDLE;

		VkCommandPool m_cmdPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_drawCmdBuffers;
		std::vector<VkFence> m_waitFences;
		VkSemaphore m_presentSemaphore, m_renderSemaphore;
		void prepareSynchronizationPrimitives(const VkDevice &device);

		Device *pDevice = nullptr;
		Swapchain *pSwapchain = nullptr;
		RenderPass *pRenderPass = nullptr;
		Pipeline *pBasicPipeline = nullptr;
		std::vector<VkFramebuffer> m_framebuffers;
		void setupFrameBuffers();

		VkImage m_depthImage = VK_NULL_HANDLE;
		VkDeviceMemory m_depthImageMem = VK_NULL_HANDLE;
		VkImageView m_depthImageView = VK_NULL_HANDLE;
		void setupDepthImageBuffer();
		
		typedef struct _deviceBufObject {
			VkBuffer buffer = VK_NULL_HANDLE;
			VkDeviceMemory memory = VK_NULL_HANDLE;
			VkMemoryRequirements memoryReq = {};
		} DeviceBufObject;
		DeviceBufObject m_verticeBuf, m_indiceBuf;
		
		void prepareVertices();
		
		DeviceBufObject m_uniformBuf;
		void setupUniformBuffers();
		
		VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
		void setupDescriptorPool();

		VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
		void setupDescriptorSet();

		void buildCommandBuffers();
		
	};
};

#endif // TKScene.h


