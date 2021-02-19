//
//TKUtility.h
//

#ifndef __TK_Utility_h__
#define __TK_Utility_h__
#include "TKPrefix.h"


namespace TK {
	class Utility{
	public:
		static VkShaderModule loadSPIVShader(const char *file, const VkDevice &device);


	};
};


#endif
