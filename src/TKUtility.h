//
//TKUtility.h
//

#ifndef __TK_Utility_h__
#define __TK_Utility_h__
#include "TKPrefix.h"
#include <vulkan/vulkan_core.h>

namespace TK {
	class Utility{
	public:
		static VkShaderModule loadSPIVShader(const char *file, const VkDevice &device);
		static VkPrimitiveTopology primitveTopology(const std::string &topologyStr);

	};

	class Configure {
	public:
		Configure();
		~Configure();
		static Configure *shared();
		VkPrimitiveTopology primTopology(const std::string &key);
		VkPolygonMode polygonMode(const std::string &key);
		VkFrontFace frontFace(const std::string &key);
		VkSampleCountFlagBits sampleCountFlagBit(int i);
	private:
		std::map<std::string, VkPrimitiveTopology> m_primTopologyDict;
		std::map<std::string, VkPolygonMode> m_polygonModeDict;
		std::map<std::string, VkFrontFace> m_frontFaceDict;
		
	};
};


#endif
