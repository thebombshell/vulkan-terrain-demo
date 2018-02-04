
// /vulkan_instance.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_INSTANCE_HPP
#define VK_TERRAIN_DEMO_VULKAN_INSTANCE_HPP

#include "vulkan.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class instance {
			
			public:
			
			instance();
			~instance();
			
			VkInstance get_instance();
			VkPhysicalDevice get_physical_device();
			int get_graphics_family_index();
			
			private:
			
			void create_instance();
			void setup_validation();
			void shutdown_validation();
			
			VkInstance m_instance;
			VkDebugReportCallbackEXT m_callback;
		};
	}
}

#endif