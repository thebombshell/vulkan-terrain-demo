
// vulkan_pipeline.hpp
//
// source file for the RAII wrrapper of the VkPipeline
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_PIPELINE_HPP
#define VKCPP_VULKAN_PIPELINE_HPP

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
		vk::pipeline_layout* m_pipeline_layout;
		vk::render_pass* m_render_pass;
		
		VkPipeline m_pipeline;
	};
}

#endif