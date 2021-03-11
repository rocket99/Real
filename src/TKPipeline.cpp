//
//TKPipeline.cpp
//

#include "TKPipeline.h"
#include "TKPrefix.h"
#include "TKUtility.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/memorystream.h>
#include <vulkan/vulkan_core.h>

using namespace TK;

Pipeline::Pipeline(){}

Pipeline::~Pipeline() {}

Pipeline *Pipeline::create(const VkDevice &device, const VkRenderPass &renderPass){
	Pipeline *pipeline = new Pipeline();
	if(pipeline->initPipeline(device, renderPass)==false){
		delete pipeline;
		return nullptr;
	}
	return pipeline;
}

bool Pipeline::initPipeline(const VkDevice &device, const VkRenderPass &renderPass){
	m_device = device;
//	this->setupPipelineCache();
	this->setupPipelineLayout();
	
	VkPipelineInputAssemblyStateCreateInfo inputAssembleState = {};
	inputAssembleState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembleState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineRasterizationStateCreateInfo rasterizationState = {};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
	rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationState.depthClampEnable = VK_FALSE;
	rasterizationState.depthBiasEnable = VK_FALSE;
	rasterizationState.lineWidth = 2.0;

	VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
	blendAttachmentState[0].colorWriteMask = 0xf;
	blendAttachmentState[0].blendEnable = VK_TRUE;
	blendAttachmentState[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentState[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
	blendAttachmentState[0].alphaBlendOp = VK_BLEND_OP_ADD;
	blendAttachmentState[0].colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachmentState[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
	blendAttachmentState[0].dstColorBlendFactor = VK_BLEND_FACTOR_DST_COLOR;
	blendAttachmentState[0].colorWriteMask = 
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT ;
		
	VkPipelineColorBlendStateCreateInfo colorBlendState;
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = blendAttachmentState;
	colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	float blendConstants[] = { 1.0, 1.0, 1.0, 1.0 };
	memcpy(colorBlendState.blendConstants, blendConstants, sizeof(float)*4);
	
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	std::vector<VkDynamicState> dynamicStateEnables;
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	dynamicState.pDynamicStates = dynamicStateEnables.data();

	VkPipelineMultisampleStateCreateInfo multisampleState = {};
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.pNext = nullptr;
	multisampleState.flags = 0;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.pSampleMask = nullptr;

	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.depthTestEnable = VK_TRUE;
	depthStencilState.depthWriteEnable = VK_TRUE;
	depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilState.depthBoundsTestEnable = VK_FALSE;
	depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
	depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
	depthStencilState.stencilTestEnable = VK_FALSE;
	depthStencilState.front = depthStencilState.back;

	VkVertexInputBindingDescription vertexInputBinding = {};
	vertexInputBinding.binding = 0;
	vertexInputBinding.stride = sizeof(Vertex);
	vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> vertexInputAttributs(2);
	vertexInputAttributs[0].binding = 0;
	vertexInputAttributs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[0].location = 0;
	vertexInputAttributs[0].offset = offsetof(Vertex, position);

	vertexInputAttributs[1].binding = 0;
	vertexInputAttributs[1].location = 1;
	vertexInputAttributs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributs[1].offset = offsetof(Vertex, color);

	VkPipelineVertexInputStateCreateInfo vertexInputState = {};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState.pNext = nullptr;
	vertexInputState.flags = 0;
	vertexInputState.vertexBindingDescriptionCount = 1;
	vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
	vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributs.size());
	vertexInputState.pVertexAttributeDescriptions = vertexInputAttributs.data();

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].pName = "main";
	shaderStages[0].module = Utility::loadSPIVShader("../shader/basic_vert.spv", device);
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

	if(shaderStages[0].module == VK_NULL_HANDLE){
		TKError("invalid shader module!");
	}
	
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].pName = "main";
	shaderStages[1].module = Utility::loadSPIVShader("basic_frag.spv", device);
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	if(shaderStages[1].module == VK_NULL_HANDLE){
		TKError("invalid shader module!");
	}
	
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.layout = VK_NULL_HANDLE;// m_pipelineLayout;
	pipelineCreateInfo.pNext = nullptr;
	
	pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pDynamicState = &dynamicState;
	pipelineCreateInfo.pColorBlendState = &colorBlendState;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;
	pipelineCreateInfo.pMultisampleState = &multisampleState;
	pipelineCreateInfo.pInputAssemblyState = &inputAssembleState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;

	VkResult ret = vkCreateGraphicsPipelines(device, m_pipelineCache, 1,
											 &pipelineCreateInfo, nullptr, &m_pipeline);	
	if(ret != VK_SUCCESS){
		TKError("create pipeline failed");
		return false;
	}
	TKLog("create pipeline");
	vkDestroyShaderModule(m_device, shaderStages[0].module, nullptr);
	vkDestroyShaderModule(m_device, shaderStages[1].module, nullptr);
	
	return true;
}

Pipeline *Pipeline::createWithJson(const VkDevice &device,
                                   const VkRenderPass &renderPass,
                                   const std::string &jsonFile) {
	Pipeline *pl = new Pipeline();
	if(pl->initWithJson(device, renderPass, jsonFile)==false){
		delete pl;
		return nullptr;
	}
	return pl;
}

bool Pipeline::initWithJson(const VkDevice &device,
							const VkRenderPass &renderPass,
							const std::string &jsonFile){
	std::ifstream srcFile(jsonFile, std::ios::in);
	if(!srcFile.is_open()){
		TKError("pipeline configure File not found! %s", jsonFile.c_str());
		return false;
	}
	char buf[256];
	std::string jsonStr = "";
	while(!srcFile.eof()){
		srcFile.getline(buf, 256);
		jsonStr += buf;
	}
	rapidjson::Document doc;
	doc.Parse(jsonStr.c_str());
	std::string nameStr = doc["name"].GetString();
	m_name = nameStr;
	
	VkPipelineInputAssemblyStateCreateInfo inputAssembleState = {};
	inputAssembleState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembleState.pNext = nullptr;
	
	if(doc["inputAssembleState"].IsObject()){
		std::string topology = doc["inputAssembleState"]["topology"].GetString();
		inputAssembleState.topology = Configure::shared()->primTopology(topology);
	}

	inputAssembleState.primitiveRestartEnable =
		doc["inputAssembleState"]["restartEnable"].GetBool() ? VK_TRUE : VK_FALSE;

	VkPipelineRasterizationStateCreateInfo rasterState = {};
	rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterState.flags = 0;
		
	if(doc.HasMember("raterizationState")){
		rapidjson::Value tmpDoc = doc["rasterizationState"].GetObject();
		rasterState.lineWidth = tmpDoc["lineWidth"].GetFloat();
		rasterState.depthBiasClamp = tmpDoc["depthClamp"].GetBool() ? VK_TRUE : VK_FALSE;
		rasterState.depthBiasEnable = tmpDoc["depthBias"].GetBool() ? VK_TRUE : VK_FALSE;
		rasterState.frontFace = Configure::shared()->frontFace(tmpDoc["frontFace"].GetString());
		rasterState.polygonMode = Configure::shared()->polygonMode(tmpDoc["polygon"].GetString());
	}

	VkPipelineViewportStateCreateInfo vpState = {};
	vpState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vpState.pNext = nullptr;
	vpState.flags = 0;
	vpState.scissorCount = doc["viewportState"]["scissorCount"].GetInt();
	vpState.scissorCount = doc["viewportState"]["viewportCount"].GetInt();
	
	this->initMultiSampleCreateInfo(doc);
	this->initDepthStencilStateCreateInfo(doc);

	
	return true;
	
}


void Pipeline::initMultiSampleCreateInfo(const rapidjson::Document &doc){
	m_mulSampleState.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_mulSampleState.pNext = nullptr;
	if(doc.HasMember("multiSampleState")){
		auto tmpDoc = doc["multiSampleState"].GetObject();
		if(tmpDoc.HasMember("psampleMask")){
			m_mulSampleState.pSampleMask = std::string(tmpDoc["pSampleMask"].GetString())=="null" ?
				nullptr : nullptr;
		}
		
		if(tmpDoc.HasMember("rasterizationSamples")){
			int value = tmpDoc["rasterizationSamples"].GetInt();
			m_mulSampleState.rasterizationSamples = Configure::shared()->sampleCountFlagBit(value);
		}

		
	}
}

void Pipeline::initDepthStencilStateCreateInfo(const rapidjson::Document &doc){
	m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_depthStencilState.pNext = nullptr;
	if(!doc.HasMember("depthStencilState")){
		return;
	}
	auto tmpDoc = doc["depthStencilState"].GetObject();
	if(tmpDoc.HasMember("depthTestEnable")){
		m_depthStencilState.depthTestEnable = tmpDoc["depthTestEnable"].GetBool() ? VK_TRUE : VK_FALSE;
	}
	if(tmpDoc.HasMember("depthWriteEnable")){
		m_depthStencilState.depthWriteEnable = tmpDoc["depthWriteEnable"].GetBool() ? VK_TRUE : VK_FALSE;
	}
	if(tmpDoc.HasMember("depthCompareOp")){
		std::string compareStr = tmpDoc["depthCompareOp"].GetString();
		if(compareStr == "less_equal"){
			m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		}
		if(compareStr == "always"){
			m_depthStencilState.depthCompareOp = VK_COMPARE_OP_ALWAYS;
		}
		if(compareStr == "greater_equal"){
			m_depthStencilState.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
		}
		if(compareStr == "less"){
			m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
		}
		if(compareStr == "greater"){
			m_depthStencilState.depthCompareOp = VK_COMPARE_OP_GREATER;
		}
	}
	if(tmpDoc.HasMember("depthBoundsTestEnable")){
		m_depthStencilState.depthBoundsTestEnable = tmpDoc["depthBoundsTestEnable"].GetBool() ? VK_TRUE : VK_FALSE;
	}
	if(tmpDoc.HasMember("stencilTestEnable")){
		m_depthStencilState.stencilTestEnable = tmpDoc["stencilTestEnable"].GetBool() ? VK_TRUE : VK_FALSE;
	}
	if(tmpDoc.HasMember("backFailOp")){
		std::string str = tmpDoc["backFailOp"].GetString();
		if(str == "keep"){
			m_depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
		}else if(str == "decrement_clamp"){
			m_depthStencilState.back.failOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		}
	}
	if(tmpDoc.HasMember("backPassOp")){
		std::string str = tmpDoc["backPassOp"].GetString();
		if(str == "keep"){
			m_depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
		}else if(str == "decrement_clamp"){
			m_depthStencilState.back.passOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		}
	}
	if(tmpDoc.HasMember("frontFailOp")){
		std::string str = tmpDoc["frontFailOp"].GetString();
		if(str == "keep"){
			m_depthStencilState.front.failOp = VK_STENCIL_OP_KEEP;
		}else if(str == "decrement_clamp"){
			m_depthStencilState.front.failOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		}
	}
	if(tmpDoc.HasMember("frontPassOp")){
		std::string str = tmpDoc["frontPassOp"].GetString();
		if(str == "keep"){
			m_depthStencilState.front.passOp = VK_STENCIL_OP_KEEP;
		}else if(str == "decrement_clamp"){
			m_depthStencilState.front.passOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		}
	}
}

void Pipeline::initDynamicStateCreateInfo(const rapidjson::Document &doc) {}



void Pipeline::destroy(){
	if(m_pipeline != VK_NULL_HANDLE){
		vkDestroyPipeline(m_device, m_pipeline, nullptr);
	}
	if(m_pipelineLayout != VK_NULL_HANDLE){
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	}
	if(m_pipelineCache != VK_NULL_HANDLE){
		vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);
	}
}

void Pipeline::setupPipelineCache(){
	VkPipelineCacheCreateInfo cacheCreateInfo = {};
	cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheCreateInfo.pNext = nullptr;
	VkResult ret = vkCreatePipelineCache(m_device, &cacheCreateInfo, nullptr, &m_pipelineCache);
	if(ret != VK_SUCCESS){
		TKError("create pipeline cache failed");
	}
}

void Pipeline::setupPipelineLayout(){
	
	VkDescriptorSetLayoutBinding layoutBinding;
	layoutBinding.binding = 0;
	layoutBinding.descriptorCount = 1;
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	
	VkDescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorLayout.pNext = nullptr;
	descriptorLayout.pBindings = &layoutBinding;
	descriptorLayout.bindingCount = 1;
	descriptorLayout.flags = 0;
	VkResult ret = vkCreateDescriptorSetLayout(m_device, &descriptorLayout, nullptr, &m_descriptorSetLayout);
	if(ret != VK_SUCCESS){
		TKError("create descriptor set layout failed!");
	}
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;//1;
	pipelineLayoutInfo.pSetLayouts = nullptr;//&m_descriptorSetLayout;

	ret = vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
	if(ret != VK_SUCCESS){
		TKError("create pipeline layout failed\n");
	}
}

VkPipeline Pipeline::pipeline() const {
	return m_pipeline;
}

VkPipelineLayout Pipeline::pipelineLayout() const {
	return m_pipelineLayout;
}

VkDescriptorSetLayout Pipeline::descriptorSetLayout() const {
	return m_descriptorSetLayout;
}
