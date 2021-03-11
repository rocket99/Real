#include "TKComputePipeline.h"
#include "TKPrefix.h"
#include "TKUtility.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	ComputePipeline::ComputePipeline():
		m_pipeline(VK_NULL_HANDLE),
		m_device(VK_NULL_HANDLE),
		m_descPool(VK_NULL_HANDLE),
		m_descSet(VK_NULL_HANDLE),
		m_descSetLayout(VK_NULL_HANDLE){}
	
	ComputePipeline::~ComputePipeline(){}

	ComputePipeline *ComputePipeline::create(const VkDevice &device, const std::string &shaderFile){
		ComputePipeline *cpl = new ComputePipeline();
		if(!cpl->init(device, shaderFile)){
			delete cpl;
			return nullptr;
		}
		return cpl;
	}

	bool ComputePipeline::init(const VkDevice &device, const std::string &shaderFile){
		m_device  = device;
		m_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		m_info.pNext = nullptr;
		m_info.flags = 0;

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.pNext = nullptr;
		shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderStageInfo.module = Utility::loadSPIVShader("../shader/basic_compute.spv", m_device);
		shaderStageInfo.pSpecializationInfo = nullptr;
		shaderStageInfo.flags = 0;
		shaderStageInfo.pName = "basic_compute";
		m_info.stage = shaderStageInfo;
		m_info.basePipelineHandle = VK_NULL_HANDLE;
		m_info.basePipelineIndex = 0;

		VkResult ret = vkCreateComputePipelines(m_device, VK_NULL_HANDLE, 1, &m_info, nullptr, &m_pipeline);
		if(ret != VK_SUCCESS){
			TKLog("create compute pipeline failed");
			return false;
		}
		return true;
	}

	bool ComputePipeline::initDescriptorSetPool(){
		std::vector<VkDescriptorPoolSize> poolSizeArr;
		VkDescriptorPoolSize poolSize;
		poolSize.descriptorCount = 1;
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizeArr.push_back(poolSize);
		
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.pNext = nullptr;
		poolInfo.flags = 0;
		poolInfo.maxSets = 1;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizeArr.size());
		poolInfo.pPoolSizes = poolSizeArr.data();

		VkResult ret = vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descPool);
		if(ret != VK_SUCCESS){
			TKError("create descriptor set pool failed! Err=%d", ret);
			return false;
		}
		return true;
	}
}
