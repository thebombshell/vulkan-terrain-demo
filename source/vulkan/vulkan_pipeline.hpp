
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
	
	class i_pipeline {
		
		public:
		
		virtual ~i_pipeline() = 0;
		
		VkPipeline get_pipeline();
		
		protected:
		
		i_pipeline();
		VkPipeline m_pipeline;
	};
	
	class graphics_pipeline : public i_device_object, public i_pipeline {
		
		public:
		
		graphics_pipeline(vk::device& t_device,  vk::render_pass& t_render_pass
			, VkExtent2D t_extent, VkFormat t_format
			, const std::vector<vk::shader_module*>& t_shader_modules
			, const vk::vertex_definition& t_definition);
		~graphics_pipeline();
		
		vk::render_pass& get_render_pass();
		const vk::render_pass& get_render_pass() const;
		
		private:
		
		std::vector<vk::shader_module*> m_shader_modules;
		vk::pipeline_layout* m_pipeline_layout;
		vk::render_pass& m_render_pass;
	};
}

#endif