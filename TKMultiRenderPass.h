//
//TKMultiRenderPass
//

#ifndef __TKMultiRenderPass_H__
#define __TKMultiRenderPass_H__

#include "TKPrefix.h"

namespace TK{
	class MultiRenderPass {
	public:
		MultiRenderPass();
		~MultiRenderPass();

		static MultiRenderPass *create(const VkDevice &device);
		bool init(const VkDevice &device);

	private:
		VkDevice m_device;
		VkRenderPass m_renderPass;
	};
};
#endif

