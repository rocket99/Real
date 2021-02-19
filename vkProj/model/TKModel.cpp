#include "TKModel.h"

#include <vulkan/vulkan_core.h>

namespace Model {
	Base::Base()
	{
		m_vertBuf=VK_NULL_HANDLE;
	}

	Base::~Base(){}
	
	Base *Base::create(){
		Base *mod = new Base();
		if(!mod->init()){
			delete mod;
		}
		return mod;
	}
	
	bool Base::init(){
		return true;
	}


	
}
