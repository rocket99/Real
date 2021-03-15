#include "TKComputePipeline.h"
#include "TKPrefix.h"
#include "TKUtility.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	ComputePipeline::ComputePipeline():
		m_pipeline(VK_NULL_HANDLE),
		m_device(VK_NULL_HANDLE),
		m_descPool(VK_NULL_HANDLE),
		m_descSet(VK_NULL_HANDLE){}
	
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
		shaderStageInfo.module = Utility::loadSPIVShader(shaderFile.c_str(), m_device);
		shaderStageInfo.pSpecializationInfo = nullptr;
		shaderStageInfo.flags = 0;
		shaderStageInfo.pName = "basic_compute";
		m_info.stage = shaderStageInfo;
		m_info.basePipelineHandle = VK_NULL_HANDLE;
		m_info.basePipelineIndex = 0;

		VkResult ret = vkCreateComputePipelines(m_device, VK_NULL_HANDLE, 1,
												&m_info, nullptr, &m_pipeline);
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

	bool ComputePipeline::initDescriptorSetLayout(){
		std::vector<VkDescriptorSetLayoutBinding> bindingArr;
		VkDescriptorSetLayoutBinding binding = {};
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		binding.pImmutableSamplers = nullptr;
		bindingArr.push_back(binding);
		
		VkDescriptorSetLayout layout = VK_NULL_HANDLE;
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;
		layoutInfo.flags = 0;
		layoutInfo.bindingCount = UINT32_T(bindingArr.size());
		layoutInfo.pBindings = bindingArr.data();
		VkResult ret = vkCreateDescriptorSetLayout(m_device, &layoutInfo,
												   nullptr, &layout);
		if(ret != VK_SUCCESS){
			TKError("create descriptor set layout failed! Err=%d\n", ret);
			return false;
		}
		m_descSetLayoutArr.push_back(layout);
		return true;
	}

	bool ComputePipeline::initDescriptorSet(){
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = m_descPool;
		allocInfo.descriptorSetCount = UINT32_T(m_descSetLayoutArr.size());
		allocInfo.pSetLayouts = m_descSetLayoutArr.data();
		vkAllocateDescriptorSets(m_device, &allocInfo, &m_descSet);
		return true;
	}

	void ComputePipeline::updateWriteDescSets(){

	}
} // namespace TK



