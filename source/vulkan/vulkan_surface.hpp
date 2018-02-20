
// vulkan_surface.hpp
//
// header file for the RAII wrapper of the vulkan surface extended classes
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_SURFACE_HPP
#define VKCPP_VULKAN_SURFACE_HPP

#include "vulkan.hpp"

#include <windows.h>

namespace vk {
	
	// vk::surface
	//
	// A CPP style RAII compliant wrapper for the VkSurfaceKHR
	class surface {
		
		public:
		
		// constructor
		//
		// param - t_window_handle - the win32 window handle to create the surface with
		// param - t_handle_instance - the win32 instance handle to create the surface with
		// param - t_instance - the vk::instance to create the surface with
		// param - t_physical_device - the vk::physical_device to associate the surface with
		surface
			( HWND t_window_handle, HINSTANCE t_handle_instance, vk::instance& t_instance, vk::physical_device& t_physical_device);
		
		// destructor
		~surface();
		
		// get_instance
		//
		// returns - a reference to the vk::instance used to create the surface with
		vk::instance& get_instance();
		const vk::instance& get_instance() const;
		
		// get_physical_device
		//
		// returns - a reference to the vk::physical_device used to create the surface with
		vk::physical_device& get_physical_device();
		const vk::physical_device& get_physical_device() const;
		
		// get_surface
		//
		// returns - the VkSurfaceKHR the class wraps up
		VkSurfaceKHR get_surface() const;
		
		// get_surface_capabilities
		//
		// returns - a VkSurfaceCapabilitiesKHR relating to this surface
		VkSurfaceCapabilitiesKHR get_surface_capabilities() const;
		
		// get_surface_formats
		//
		// returns - a collection of VkSurfaceFormatKHR's relating to this surface
		const std::vector<VkSurfaceFormatKHR>& get_surface_formats() const;
		
		// get_present_modes
		//
		// returns - a collection of VkPresentModeKHR's relating to this surface
		const std::vector<VkPresentModeKHR>& get_present_modes() const;
		
		// is_surface_supported_by_queue_family
		//
		// param - t_queue_family_index - the index of the queue family to check for support
		// returns - true if the surface is supported by the specified queue family
		VkBool32 is_surface_supported_by_queue_family(uint32_t t_queue_family_index) const;
		
		private:
		
		HWND m_window_handle;
		HINSTANCE m_instance_handle;
		vk::instance& m_instance;
		vk::physical_device& m_physical_device;
		
		VkSurfaceKHR m_surface;
		VkSurfaceCapabilitiesKHR m_surface_capabilities;
		std::vector<VkSurfaceFormatKHR> m_surface_formats;
		std::vector<VkPresentModeKHR> m_present_modes;
	};
}

#endif