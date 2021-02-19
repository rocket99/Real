//
//TKMultiRenderPass.h
//

#include "TKMultiRenderPass.h"

using namespace TK;

MultiRenderPass::MultiRenderPass(){

}

MultiRenderPass::~MultiRenderPass(){

}

MultiRenderPass *MultiRenderPass::create(const VkDevice &device){
	MultiRenderPass *rp = new MultiRenderPass();
	if(rp->init(device)==false){
		delete rp;
		return nullptr;
	}
	return rp;
}

bool MultiRenderPass::init(const VkDevice &device){
	if(device == VK_NULL_HANDLE){
		return false;
	}
	m_device = device;

	std::array<VkAttachmentDescription, 2> attchDesc;
	
	
	return true;
}
