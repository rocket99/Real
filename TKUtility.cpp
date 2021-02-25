#include "TKUtility.h"
#include <fstream>
#include <vulkan/vulkan_core.h>

namespace TK{
	VkShaderModule Utility::loadSPIVShader(const char *file, const VkDevice &device){
		std::ifstream st(file, std::ios::binary | std::ios::in | std::ios::ate);
		if(st.is_open()){
			size_t size = st.tellg();
			if(size%4 != 0){
				TKerror("invalid shader spv file!");
				return VK_NULL_HANDLE;
			}
			st.seekg(0, std::ios::beg);
			
			char *shaderCode = new char[size];
			st.read(shaderCode, size);
			
			VkShaderModule module;
			VkShaderModuleCreateInfo moduleCreateInfo = {};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = size;
			moduleCreateInfo.pCode = (uint32_t *)shaderCode;
			VkResult ret = vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &module);
			if(ret != VK_SUCCESS){
				TKerror("create shader module failed from %s", file);
			}
			delete [] shaderCode;
			st.close();
			return module;
		}
	
		TKerror("Failed to open file: %s", file);
		return VK_NULL_HANDLE;
	}

	VkPrimitiveTopology Utility::primitveTopology(const std::string &topologyStr){
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}


	static Configure *conf = nullptr;
	Configure::Configure(){
		m_primTopologyDict["triangle_list"] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_primTopologyDict["triangle_fan"] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		m_primTopologyDict["triangle_list_adjacency"] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
		m_primTopologyDict["line_list"] = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		m_primTopologyDict["line_strip"] = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		m_primTopologyDict["line_list_adjacency"] = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
		m_primTopologyDict["line_strip_adjaceny"] = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
		m_primTopologyDict["point_list"] = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		m_primTopologyDict["triangle_strip"] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		m_primTopologyDict["triangle_trip_adjacency"] = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
	}
	Configure::~Configure(){}

	Configure *Configure::shared(){
		if(conf == nullptr){
			conf = new Configure();
		}
		return conf;
	}

	std::map<std::string, VkPrimitiveTopology> &Configure::primTopologyMap(){
		return  m_primTopologyDict;
	}
};



