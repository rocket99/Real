//
//TKUsdLoader.cpp
//

#include "TKUsdLoader.h"

using namespace TK;

UsdLoader::UsdLoader(){

}

UsdLoader::~UsdLoader(){

}

UsdLoader *UsdLoader::create(const std::string &usdFileName) {
	UsdLoader *loader = new UsdLoader();
	if(loader->initUsdFile(usdFileName)==false){
		delete loader;
		return nullptr;
	}
	return loader;
}

bool UsdLoader::initUsdFile(const std::string &usdFileName){
//	UsdStage *stage = UsdStage::Open();
	return true;
}


