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
		static ComputePipeline *create();
		bool init();
	private:
		VkPipeline m_pipeline;
	};
};


#endif


