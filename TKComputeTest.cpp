#include "TKComputeTest.h"
#include "TKPrefix.h"
#include <vulkan/vulkan_core.h>

namespace TK{
	ComputeTest::ComputeTest(){}
	ComputeTest::~ComputeTest(){}

	ComputeTest *ComputeTest::Create(Device *device, const std::string &shaderFile){
		ComputeTest *test = new ComputeTest();
		if(!test->init(device, shaderFile)){
			delete test;
			return nullptr;
		}
		return test;
	}

	bool ComputeTest::init(Device *device, const std::string &shaderFile){
		pPipeline = ComputePipeline::create(device->logicalDevice(), "../shader/basic_compute.spv");
		m_cmdPool = device->createCommandPool(device->computeQueueFamilyIndex());
		m_cmdBuf = device->createCommandBuffer(m_cmdPool);

		this->initDescriptorSet(device->logicalDevice());
		
		return true;
	}

	bool ComputeTest::initDescriptorSet(const VkDevice &device){
		VkDescriptorPoolCreateInfo descPoolInfo = {};
		descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descPoolInfo.pNext = nullptr;
		descPoolInfo.flags = 0;
		descPoolInfo.maxSets = 1;
		std::vector<VkDescriptorPoolSize> poolSizeArr;
		VkDescriptorPoolSize size;
		size.descriptorCount = 1;
		size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizeArr.push_back(size);
		descPoolInfo.pPoolSizes = poolSizeArr.data();
		descPoolInfo.poolSizeCount = poolSizeArr.size();
		VkResult ret = vkCreateDescriptorPool(device, &descPoolInfo, nullptr, &m_descPool);
		if(ret != VK_SUCCESS){
			TKerror("create descriptor pool failed! Err=%d", ret);
			return false;
		}
		
		VkDescriptorSetLayoutCreateInfo setLayoutInfo = {};
		setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setLayoutInfo.pNext = nullptr;
		setLayoutInfo.bindingCount = 3;
		setLayoutInfo.flags = 0;
		std::vector<VkDescriptorSetLayoutBinding> bindingArr;
		VkDescriptorSetLayoutBinding binding0;
		binding0.binding = 0;
		binding0.descriptorCount = 1;
		binding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		binding0.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		bindingArr.push_back(binding0);
		setLayoutInfo.pBindings = nullptr;
		vkCreateDescriptorSetLayout(device, &setLayoutInfo, nullptr, &m_descSetLayout);
		return true;
	}
};
