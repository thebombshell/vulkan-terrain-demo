
// vulkan_context.hpp
//
// header file for the VKCPP all encompassing graphical context, intended to contain complex vulkan procedures and make
// graphics experiments easier and faster to produce
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VK_TERRAIN_DEMO_VULKAN_CONTEXT_HPP
#define VK_TERRAIN_DEMO_VULKAN_CONTEXT_HPP

#include "vulkan.hpp"

#include <windows.h>

namespace vk {

	class context {
		
		public:
		
		context(HWND t_window_handle, HINSTANCE t_instance_handle);
		~context();
		
		vk::device& get_device();
		const vk::device& get_device() const;
		
		vk::swapchain& get_swapchain();
		const vk::swapchain& get_swapchain() const;
		
		private:
		
		vk::instance* m_instance;
		vk::validation* m_validation;
		vk::surface* m_surface;
		vk::device* m_device;
		vk::swapchain* m_swapchain;
		std::vector<vk::framebuffer*> m_framebuffers;
	};
}

#endif