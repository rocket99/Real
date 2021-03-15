//
//TKComputePileine.h
//
//
#ifndef __TKComputePipeine_h__
#define __TKComputePipeline_h__

#include "TKPrefix.h"

namespace TK{
	class ComputePipeline {
	public:
		ComputePipeline();
		~ComputePipeline();
		static ComputePipeline *create(const VkDevice &device,
									   const std::string &shaderFile);
		bool init(const VkDevice &device, const std::string &shaderFile);
	private:
		VkDevice m_device;
		VkPipeline m_pipeline;
		VkComputePipelineCreateInfo m_info = {};

		VkDescriptorPool m_descPool;
		std::vector<VkDescriptorSetLayout> m_descSetLayoutArr;
		VkDescriptorSet m_descSet;
		bool initDescriptorSetPool();

		bool initDescriptorSetLayout();
		bool initDescriptorSet();
		void updateWriteDescSets();

	};
};

#endif

