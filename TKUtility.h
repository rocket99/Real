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
		std::map<std::string, VkPrimitiveTopology> &primTopologyMap();
	private:
		std::map<std::string, VkPrimitiveTopology> m_primTopologyDict;
	};
};


#endif
