//
//TKMaterial.cpp
//

#include "TKMaterial.h"

namespace TK {
	Material::Material(){}
	Material::~Material(){}

	Material *Material::create(const std::string &name, const std::string &configJson){
		Material *mtl = new Material();
		if(mtl->init(name, configJson)==false){
			delete mtl;
			return nullptr;
		}
		return mtl;
	}

	bool Material::init(const std::string &name, const std::string &configJson){

		return true;
	}
	
};
