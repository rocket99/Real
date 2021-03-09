//
//TKPipeline.h
//

#ifndef __TKPipeline_h__
#define __TKPipeline_h__

#include "TKPrefix.h"
#include <rapidjson/document.h>
#include <vulkan/vulkan_core.h>

namespace TK {

	class Pipeline {
	public:
		Pipeline();
		~Pipeline();
		static Pipeline *create(const VkDevice &device, const VkRenderPass &renderPass);
		bool initPipeline(const VkDevice &device, const VkRenderPass &renderPass);

		static Pipeline *createWithJson(const VkDevice &device, const VkRenderPass &renderPass, const std::string &jsonFile);
		bool initWithJson(const VkDevice &device, const VkRenderPass &renderPass, const std::string &jsonFile);

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


		VkPipelineMultisampleStateCreateInfo m_mulSampleState = {};
		void initMultiSampleCreateInfo(const rapidjson::Document &doc);

		VkPipelineDepthStencilStateCreateInfo m_depthStencilState = {};
		void initDepthStencilStateCreateInfo(const rapidjson::Document &doc);

		VkPipelineDynamicStateCreateInfo m_dynamicState = {};
		void initDynamicStateCreateInfo(const rapidjson::Document &doc);

		VkPipelineInputAssemblyStateCreateInfo m_inputAssembleState = {};
		void initInputAssembleStateCreateInfo(const rapidjson::Document &doc);

		VkPipelineVertexInputStateCreateInfo m_vertexInputState = {};
		void initVertexInputStateCreateInfo(const rapidjson::Document &doc);

		VkPipelineShaderStageCreateInfo m_shaderStageState = {};
		void initShaderStageCreateInfo(const rapidjson::Document &doc);

		VkPipelineViewportStateCreateInfo m_viewportState = {};
		void initViewportStateCreateInfo(const rapidjson::Document &doc);
		
		std::string m_name;
	};
 
}; // namespace TK
#endif

