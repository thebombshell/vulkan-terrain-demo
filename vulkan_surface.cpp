
// /vulkan_surface.cpp

#include "vulkan_surface.hpp"
#include "vulkan_instance.hpp"

using namespace vk_terrain_demo;

vk::surface::surface(window& t_window, vk::instance& t_instance) : m_window{t_window}, m_instance{t_instance} {
	
	create_surface();
}

vk::surface::~surface() {
	
	vkDestroySurfaceKHR(m_instance.get_instance(), m_surface, nullptr);
}

vk::instance& vk::surface::get_instance() {
	
	return m_instance;
}

VkSurfaceKHR vk::surface::get_surface() {
	
	return m_surface;
}

void vk::surface::create_surface() {
	
	VkWin32SurfaceCreateInfoKHR surface_info = {};
	surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_info.pNext = nullptr;
	surface_info.flags = 0;
	surface_info.hinstance = GetModuleHandle(nullptr);
	surface_info.hwnd = m_window.get_handle();
	auto create_win32_surface = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(m_instance.get_instance(), "vkCreateWin32SurfaceKHR");
	
	if (create_win32_surface) {
		
		VK_DEBUG
			( create_win32_surface
			, "Failed to create win32 surface"
			, m_instance.get_instance(), &surface_info, nullptr, &m_surface)
	}
	else {
		
		throw std::runtime_error("Failed to get instance procedure address.");
	}
}
