//
//TKPipeline.h
//

#ifndef __TKPipeline_h__
#define __TKPipeline_h__

#include "TKPrefix.h"

namespace TK {

	class Pipeline {
	public:
		Pipeline();
		~Pipeline();
		static Pipeline *create(const VkDevice &device, const VkRenderPass &renderPass);
		bool initPipeline(const VkDevice &device, const VkRenderPass &renderPass);
		void destroy();
		
		VkPipeline pipeline() const;
		VkDescriptorSetLayout descriptorSetLayout() const;
		VkPipelineLayout pipelineLayout() const;
	protected:
		VkDevice m_device = VK_NULL_HANDLE;
		VkPipeline m_pipeline = VK_NULL_HANDLE;
		
		VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
		void setupPipelineCache();

		VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
		void setupPipelineLayout();
	};
 
}; // namespace TK
#endif

