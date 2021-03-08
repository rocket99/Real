//
//TKComputePileine.h
//

#include <vulkan/vulkan_core.h>
#ifndef __TKComputePipeine_h__
#define __TKComputePipeline_h__

#include "TKPrefix.h"

namespace TK{
	class ComputePipeline {
	public:
		ComputePipeline();
		~ComputePipeline();
		static ComputePipeline *create(const VkDevice &device);
		bool init(const VkDevice &device);
	private:
		VkDevice m_device;
		VkPipeline m_pipeline;
		VkComputePipelineCreateInfo m_info = {};
	};
};


#endif


