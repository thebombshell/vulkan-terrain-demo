
// /window.hpp

#ifndef VK_TERRAIN_DEMO_WINDOW_HPP
#define VK_TERRAIN_DEMO_WINDOW_HPP

#include <tchar.h>
#include <windows.h>

namespace vk_terrain_demo {
	
	class window {
		
		public:
		
		window(LPCSTR t_title, int t_width, int t_height);
		~window();
		
		void run();
		
		HWND get_handle();
		private:
		
		HWND m_handle;
	};
}

#endif