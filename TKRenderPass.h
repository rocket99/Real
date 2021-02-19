//
//TKRenderpass.h
//

#ifndef __TK_Renderpass_h__
#define __TK_Renderpass_h__

#include "TKPrefix.h"

namespace TK {
	class RenderPass {
	public:
		RenderPass();
		~RenderPass();

		static RenderPass *create(const VkDevice &device, VkFormat colorFormat, VkFormat depthFormat);
		bool initRenderPass(const VkDevice &device, VkFormat colorformat, VkFormat depthFormat);
		void destroy();
		
		VkRenderPass renderPass() const;
		
	private:
		VkRenderPassCreateInfo m_createInfo;
		VkRenderPass m_renderPass;
		VkDevice m_device;
	};
};

#endif // TKRenderPass.h

