#include "TKMemManager.h"

namespace TK{
	MemManager::MemManager():
		pUniformPool(nullptr),
		pVerticePool(nullptr){}

	MemManager::~MemManager(){}

	MemManager* MemManager::Create(){
		MemManager *manager = new MemManager();
		return manager;
	}

	
};
