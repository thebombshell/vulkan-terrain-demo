
// /vulkan_surface.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_SURFACE_HPP
#define VK_TERRAIN_DEMO_VULKAN_SURFACE_HPP

#include "vulkan.hpp"
#include "window.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class surface {
			
			public:
			
			surface(window& t_window, vk::instance& t_instance);
			~surface();
			
			vk::instance& get_instance();
			
			VkSurfaceKHR get_surface();
			
			private:
			
			void create_surface();
			int find_present_family(VkPhysicalDevice& t_device);
			
			window& m_window;
			vk::instance& m_instance;
			
			VkSurfaceKHR m_surface;
		};
	}
}

#endif