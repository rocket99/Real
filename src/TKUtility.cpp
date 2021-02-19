#include "TKUtility.h"
#include <fstream>

using namespace TK;

VkShaderModule Utility::loadSPIVShader(const char *file, const VkDevice &device)
{
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




