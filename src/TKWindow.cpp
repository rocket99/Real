#include "TKWindow.h"
#include "TKDevice.h"
#include "TKPrefix.h"
#include "TKSwapchain.h"
#include "TKScene.h"

using namespace TK;

Window::Window(){
	pWindow = nullptr;
	m_vkInst = VK_NULL_HANDLE;
	m_surface = VK_NULL_HANDLE;
}

Window::~Window(){

}

Window * TK::Window::create(const std::string &name, uint32_t w, uint32_t h) {
	Window *win = new Window();
	if(win->init(name, w, h)==false){
		delete win;
		return nullptr;
	}
	return win;
}

bool Window::init(const std::string &name, uint32_t w, uint32_t h){
	int ret = SDL_Init(SDL_INIT_EVERYTHING);
	if(ret != 0){
		TKError("SDL init failed");
		return false;
	}
	pWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							   w, h, SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN| SDL_WINDOW_VULKAN);
	if(pWindow == nullptr){
		TKError("SDL create window error!");
		return false;
	}
	
/*
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(pRenderer, 127, 127, 127, 255);
	SDL_RenderClear(pRenderer);
	SDL_RenderPresent(pRenderer);
*/
	
	this->initVkInstanceSurface();
	
	pScene = Scene::create(m_vkInst, m_surface);
	if(pScene == nullptr){
		TKLog("create scene failed!\n");
		return false;
	}
	pScene->setSize(w, h);
	
	return true;
}

void Window::startLoop(){
	m_isRunning = true;
	while(m_isRunning){
		SDL_Event event;
		SDL_PollEvent(&event);
		switch(event.type){
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			this->handleKeyEvent(event.key);
			break;
		case SDL_WINDOWEVENT:
			this->handleWindowEvent(event.window);
			break;
		default:

			break;
		}

		pScene->draw();

/*
		SDL_SetRenderDrawColor(pRenderer, 127, 200, 127, 100);
		SDL_RenderClear(pRenderer);
		
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 200;
		rect.h = 200;
		SDL_SetRenderDrawColor(pRenderer, 255, 200, 127, 100);
		SDL_RenderFillRect(pRenderer, &rect);
		SDL_RenderPresent(pRenderer);
*/
		
		SDL_Delay(1);
	}
}

void Window::handleWindowEvent(const SDL_WindowEvent &event){
	switch(event.event){
	case SDL_WINDOWEVENT_CLOSE:

		break;
	case SDL_WINDOWEVENT_LEAVE:

		break;
	case SDL_WINDOWEVENT_RESIZED:
		//SDL_RenderClear(pRenderer);
		//SDL_RenderPresent(pRenderer);
		break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
	{
		int width, height;
		SDL_GetWindowSize(pWindow, &width, &height);
		m_width = width;
		m_height = height;
	}
	    break;
	default:
		break;
	};
}

void Window::handleKeyEvent(const SDL_KeyboardEvent &event){
	switch(event.type){
	case SDL_KEYDOWN:
		if(event.keysym.scancode == SDL_SCANCODE_ESCAPE){
			m_isRunning = false;
		}
		break;
	case SDL_KEYUP:

		break;
	default:
		break;
	};
}

void Window::destroy(){
	TKLog("destroy sdl window.");
	((Scene *)pScene)->destroy();
	SDL_DestroyWindow(pWindow);
	SDL_DestroyRenderer(pRenderer);
	SDL_Quit();
}

void Window::initVkInstanceSurface(){
	uint32_t extension_count;
	std::vector<const char *> extensions;
	SDL_Vulkan_GetInstanceExtensions(pWindow, &extension_count, nullptr);
	extensions.resize(extension_count);
	SDL_Vulkan_GetInstanceExtensions(pWindow, &extension_count, extensions.data());

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_API_VERSION_1_2;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;
	appInfo.pEngineName = "VkEngine";
	appInfo.pApplicationName = "vkProj";

	VkInstanceCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pApplicationInfo = &appInfo;
	info.enabledExtensionCount = extensions.size();
	info.ppEnabledExtensionNames = extensions.data();
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = nullptr;

	VkResult ret = vkCreateInstance(&info, nullptr, &m_vkInst);
	if(ret != VK_SUCCESS){
		TKError("create instance error %d", ret);
	}else{
		TKLog("create instance success!");
	}

	if(!SDL_Vulkan_CreateSurface(pWindow, m_vkInst, &m_surface)){
		TKError("create surface error!");
	}else{
		TKLog("create surface success!");
	}

	
}

const VkInstance &Window::instance() const {
	return m_vkInst;
}

const VkSurfaceKHR &Window::surface() const {
	return m_surface;
}

const VkPhysicalDevice &Window::device() const {
	return m_physicalDevices[0];
}


