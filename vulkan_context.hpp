
// vulkan_context.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_CONTEXT_HPP
#define VK_TERRAIN_DEMO_VULKAN_CONTEXT_HPP

#include "vulkan.hpp"
#include "window.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
	
		class context {
			
			public:
			
			context(window& t_window);
			~context();
			
			private:
			
			int is_validation_supported();
			
			window& m_window;
			vk::instance* m_instance;
			vk::surface* m_surface;
			vk::device* m_device;
			vk::swapchain* m_swapchain;
			vk::graphics_pipeline* m_graphics_pipeline;
		};
	}
}

#endif