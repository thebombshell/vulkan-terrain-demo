
// /vulkan_graphics_pipeline.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP
#define VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP

#include "vulkan.hpp"

namespace vk {
	
	class graphics_pipeline {
		
		public:
		
		graphics_pipeline(vk::device& t_device, vk::swapchain& t_swapchain);
		~graphics_pipeline();
		
		private:
		
		void create_pipeline();
		
		vk::device& m_device;
		vk::swapchain& m_swapchain;
		vk::shader_module* m_vertex_shader_module;
		vk::shader_module* m_fragment_shader_module;
		
		VkPipelineLayout m_pipeline_layout;
		VkRenderPass m_render_pass;
		VkPipeline m_pipeline;
	};
}

#endif