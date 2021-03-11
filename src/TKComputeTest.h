//
//TKComputeTest.h
//

#ifndef __TKComputeTest_h__
#define __TKComputeTest_h__

#include "TKComputePipeline.h"
#include "TKDevice.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	class ComputeTest {
	public:
		ComputeTest();
		~ComputeTest();
		static ComputeTest *Create(Device *device, const std::string &shaderFile);
		bool init(Device *device, const std::string &shaderFile);
	private:
		ComputePipeline *pPipeline;
		VkCommandPool m_cmdPool;
		VkCommandBuffer m_cmdBuf;
		VkDescriptorPool m_descPool;
		VkDescriptorSet m_descSet;
		VkDescriptorSetLayout m_descSetLayout;

		bool initDescriptorSet(const VkDevice &device);
	};
}

#endif

