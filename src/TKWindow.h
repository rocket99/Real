//
//TKWindow.h
//
#ifndef __TKWindow_h__
#define __TKWindow_h__

#include "TKPrefix.h"
#include "TKScene.h"

namespace TK {
	class Window {
	public:
		Window();
		~Window();
		static Window *create(const std::string &name, uint32_t w, uint32_t h);
		bool init(const std::string &name, uint32_t w, uint32_t h);

		void startLoop();
		void destroy();

		void initVkInstanceSurface();
		const VkInstance &instance() const;
		const VkSurfaceKHR &surface() const;

		const VkPhysicalDevice &device() const;

		void setScene(Scene *ps);
	protected:
		SDL_Window *pWindow;
		SDL_Renderer *pRenderer;
		SDL_Surface *pSurface;
		uint32_t m_width, m_height;
		bool m_isRunning;

		VkInstance m_vkInst;
		VkSurfaceKHR m_surface; 
		std::vector<VkPhysicalDevice> m_physicalDevices;

		void handleWindowEvent(const SDL_WindowEvent &event);
		void handleKeyEvent(const SDL_KeyboardEvent &event);

		Scene *pScene = nullptr;
	};
};

#endif // TKWindow.h


