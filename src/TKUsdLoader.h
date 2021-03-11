//
//TKUsdLoader.h
//

#ifndef __TK_USDLOADER_H__
#define __TK_USDLOADER_H__

#include "TKPrefix.h"
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>

namespace TK {
	class UsdLoader {
	public:
		UsdLoader();
		~UsdLoader();

		static UsdLoader *create(const std::string &usdFileName);
		bool initUsdFile(const std::string &usdFileName);

	private:
		
	};
};



#endif


