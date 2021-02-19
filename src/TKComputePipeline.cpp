#include "TKComputePipeline.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	ComputePipeline::ComputePipeline():
		m_pipeline(VK_NULL_HANDLE){}
	
	ComputePipeline::~ComputePipeline(){}

	ComputePipeline *ComputePipeline::create(){
		ComputePipeline *cpl = new ComputePipeline();
		if(!cpl->init()){
			delete cpl;
			return nullptr;
		}
		return cpl;
	}

	bool ComputePipeline::init(){

		return true;
	}
	

}
