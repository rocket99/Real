#include "Memory/TKMemory.h"

namespace TK {
	class MemManager{
	public:
		MemManager();
		~MemManager();
		static MemManager *Create();
	private:
		MemPool *pUniformPool;
		MemPool *pVerticePool; 
	};
};
