
// /vulkan_graphics_pipeline.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP
#define VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP

#include "vulkan.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class graphics_pipeline {
			
			public:
			
			graphics_pipeline(vk::device& t_device) ;
			~graphics_pipeline();
			
			private:
			
			void create_shader_module_from_file(const char* t_path, VkShaderModule& t_shader_module);
			
			vk::device& m_device;
			
			VkShaderModule m_vertex_module;
			VkShaderModule m_fragment_module;
		};
	}
}

#endif