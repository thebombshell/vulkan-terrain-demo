
// vulkan_pipeline.hpp
//
// source file for the RAII wrapper of the VkPipeline
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_PIPELINE_HPP
#define VKCPP_VULKAN_PIPELINE_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class pipeline {
		
		public:
		
		pipeline();
		virtual ~pipeline() = 0;
		
		VkPipeline get_pipeline();
		
		protected:
		
		VkPipeline m_pipeline;
	};
	
	class graphics_pipeline : public device_object, public pipeline {
		
		public:
		
		graphics_pipeline(vk::device& t_device, vk::swapchain& t_swapchain);
		~graphics_pipeline();
		
		vk::render_pass& get_render_pass();
		const vk::render_pass& get_render_pass() const;
		
		private:
		
		void create_pipeline();
		
		vk::swapchain& m_swapchain;
		vk::shader_module* m_vertex_shader_module;
		vk::shader_module* m_fragment_shader_module;
		vk::pipeline_layout* m_pipeline_layout;
		vk::render_pass* m_render_pass;
	};
}

#endif