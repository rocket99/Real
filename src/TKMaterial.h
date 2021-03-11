//
// TKMaterial.h
//

#ifndef __TKMaterial_h__
#define __TKMaterial_h___

#include "TKDevice.h"
#include "TKBuffer.h"
namespace TK{

	class Material {
	public:
		Material();
		~Material();
		static Material *create(const std::string &name, const std::string &configJson);
		bool init(const std::string &name, const std::string &configJson);
	protected:
		std::string m_name;
		VkPipeline m_pipeline;
		Buffer *pBuffer;
	};
};

#endif //__TKMaterial_h__
