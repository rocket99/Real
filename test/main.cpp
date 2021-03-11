#include "../src/TKWindow.h"
#include "../src/TKMemory.h"

int main(int argc, char *argv[])
{
	/*
	TK::MemPool *Pool = TK::MemPool::create(1024*1024); 
	int *buffer = (int *)Pool->Malloc(sizeof(int)*1024);
	for(int i=0; i<1024; ++i){
		buffer[i] = i;
	}
	Pool->Free(buffer);
	*/
	TK::Window *window = TK::Window::create("main", 1280, 1024);
	if(window == nullptr){
		TKLog("create window failed!\n");
		return -1;
	}
	window->startLoop();
	window->destroy();
	return 0;
}
