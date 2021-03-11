#include "TKMemory.h"
#include <cstdlib>

namespace TK {
	MemPool::MemPool():
		pDataPool(nullptr),
		m_size(0){}
	
	MemPool::~MemPool(){}

	MemPool *MemPool::create(uint64_t size){
		MemPool *pool = new MemPool();
		if(!pool->initWithSize(size)){
			delete pool;
			return nullptr;
		}
		return pool;
	}

	bool MemPool::initWithSize(uint64_t size){
		m_size = size;
		pDataPool = (int*)malloc(sizeof(int)*size);
		if(pDataPool == nullptr){
			return false;
		}
		return true;
	}

	void *MemPool::Malloc(uint64_t size){
		Record record = {};
		uint64_t S = size%4==0 ? size : (size/4+1)*4;
		std::list<Record>::iterator it0 = m_recordList.begin();
		if(it0 == m_recordList.end()){
			record.offset = 0;
			record.size = size;
		}
		while(it0 != m_recordList.end()){
			std::list<Record>::iterator it1 = it0++;
			if(it1 != m_recordList.end()){
				if((it1->offset - (it0->offset+it0->size)) > S){
					record.offset = it0->offset;
					record.size = S;
				}
			}else{
				record.offset = it0->offset;
				record.size = size;
			}
		}
		if(record.offset + record.size > m_size){
			return nullptr;
		}
		m_recordList.push_back(record);
		return (void *)&pDataPool[record.offset];
	}

	void MemPool::Free(void *ptr){
		uint64_t offset = (char *)ptr - (char *)&pDataPool[0];
		std::list<Record>::iterator it0 = m_recordList.begin();
		while(it0 != m_recordList.end()){
			if(it0->offset == offset){
				memset(&pDataPool[offset], 0, it0->size/4);
				m_recordList.erase(it0);
				break;
			}
			++ it0;
		}
	}
	
}
