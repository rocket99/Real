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

		m_polygonModeDict["fill"] = VK_POLYGON_MODE_FILL;
		m_polygonModeDict["line"] = VK_POLYGON_MODE_LINE;
		m_polygonModeDict["point"] = VK_POLYGON_MODE_POINT;
		m_polygonModeDict["fill_rect_nv"] = VK_POLYGON_MODE_FILL_RECTANGLE_NV;

		m_frontFaceDict["clock"] = VK_FRONT_FACE_CLOCKWISE;
		m_frontFaceDict["count_clock"] = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}
	
	Configure::~Configure(){}

	Configure *Configure::shared(){
		if(conf == nullptr){
			conf = new Configure();
		}
		return conf;
	}

	VkPrimitiveTopology Configure::primTopology(const std::string &key){
		TKAssert(m_primTopologyDict.find("key") != m_primTopologyDict.end(),
				 "Unsupported Primitive Topology %s", key.c_str());
		return  m_primTopologyDict[key];
	}

	VkPolygonMode Configure::polygonMode(const std::string &key){
		TKAssert(m_polygonModeDict.find(key) != m_polygonModeDict.end(),
				 "Unsupported Polygon Mode %s", key.c_str());
		return m_polygonModeDict[key];
	}

	VkFrontFace Configure::frontFace(const std::string &key){
		TKAssert(m_frontFaceDict.find(key)!=m_frontFaceDict.end(),
				 "Unsupported Front Face %s", key.c_str());
		return m_frontFaceDict[key];
	}

	VkSampleCountFlagBits Configure::sampleCountFlagBit(int flag){
		switch(flag){
		case 1:
			return VK_SAMPLE_COUNT_1_BIT;
			break;
		case 2:
			return VK_SAMPLE_COUNT_2_BIT;
			break;
		case 4:
			return VK_SAMPLE_COUNT_4_BIT;
			break;
		case 8:
			return VK_SAMPLE_COUNT_8_BIT;
			break;
		case 16:
			return VK_SAMPLE_COUNT_16_BIT;
			break;
		case 32:
			return VK_SAMPLE_COUNT_32_BIT;
			break;
		case 64:
			return VK_SAMPLE_COUNT_64_BIT;
			break;
		default:
			break;
		}
		return VK_SAMPLE_COUNT_1_BIT;
	}
	
};



