
#ifndef __TKMemroy_h__
#define __TKMemroy_h__

#include "TKPrefix.h"

namespace TK{
	class MemPool {
	public:
		MemPool();
		~MemPool();
		static MemPool *create(uint64_t size);
		bool initWithSize(uint64_t size);

		void * Malloc(uint64_t size);
		void Free(void *ptr);
	private:
		int *pDataPool;
		uint64_t m_size;
		typedef struct record_ {
			uint64_t offset;
			uint32_t size;
		} Record;
		std::list<Record> m_recordList;
	};
};

#endif
