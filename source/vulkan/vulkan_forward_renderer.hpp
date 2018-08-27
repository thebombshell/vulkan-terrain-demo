
// vulkan_forward_renderer.hpp
//
// header file for the RAII forward renderer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_FORWARD_RENDERER_HPP
#define VKCPP_VULKAN_FORWARD_RENDERER_HPP

#include "vulkan.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include <mingw.mutex.h>

#ifndef VK_RENDERER_MAX_CONCURRENT_FRAMES
#define VK_RENDERER_MAX_CONCURRENT_FRAMES 4
#endif

namespace vk {
	
	class forward_renderer : public i_device_object {
		
		public:
		
		class draw_object;
		
		forward_renderer(vk::context& t_vulkan);
		~forward_renderer();
		
		const vk::forward_renderer::draw_object* create_draw
			( vk::i_buffer& t_index_buffer, std::vector<vk::i_buffer*>& t_vertex_buffers
			, uint32_t t_element_count, VkIndexType t_index_types); 
		void render();
		
		vk::context& get_context();
		const vk::context& get_context() const;
		
		vk::render_pass& get_render_pass();
		const vk::render_pass& get_render_pass() const;
		
		private:
		
		uint32_t frame_index {0};
		vk::context& m_context;
		vk::command_pool* m_graphics_pool;
		
		vk::shader_module* m_default_vertex_shader;
		vk::shader_module* m_default_fragment_shader;
		vk::render_pass* m_render_pass;
		vk::graphics_pipeline* m_default_pipeline;
		
		std::vector<vk::framebuffer*> m_framebuffers;
		
		std::vector<vk::render_frame*> m_frames;
		std::vector<vk::render_frame*>::iterator m_frame_iterator;
		
		std::vector<vk::forward_renderer::draw_object*> m_draws;
	};
}

#endif