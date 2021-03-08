#include "TKComputePipeline.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	ComputePipeline::ComputePipeline():
		m_pipeline(VK_NULL_HANDLE){}
	
	ComputePipeline::~ComputePipeline(){}

	ComputePipeline *ComputePipeline::create(const VkDevice &device){
		ComputePipeline *cpl = new ComputePipeline();
		if(!cpl->init(device)){
			delete cpl;
			return nullptr;
		}
		return cpl;
	}

	bool ComputePipeline::init(const VkDevice &device){
		m_device  = device;
		m_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		m_info.pNext = nullptr;
		m_info.flags = 0;
		
		return true;
	}
}
