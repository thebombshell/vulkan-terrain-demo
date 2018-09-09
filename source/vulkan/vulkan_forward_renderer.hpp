
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
#include <map>
#include <mingw.mutex.h>

#ifndef VK_RENDERER_MAX_CONCURRENT_FRAMES
#define VK_RENDERER_MAX_CONCURRENT_FRAMES 4
#endif

namespace vk {
	
	class forward_renderer : public i_device_object {
		
		public:
		
		class draw_object;
		class frame_object {
			
			friend vk::forward_renderer;
			
			public:
			
			std::vector<vk::descriptor_set*>& get_descriptor_sets();
			vk::buffer& get_camera_uniform_buffer();
			
			private:
			
			frame_object(vk::descriptor_pool& t_pool, vk::descriptor_set_layout& t_layout);
			~frame_object();
			
			std::vector<vk::descriptor_set*> m_descriptor_sets;
			vk::buffer* m_camera_uniform_buffer;
		};
		
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
		std::vector<vk::descriptor_set_layout*> m_default_descriptor_layouts;
		vk::descriptor_pool* m_descriptor_pool;
		vk::pipeline_layout* m_default_layout;
		vk::graphics_pipeline* m_default_pipeline;
		
		std::vector<vk::image*> m_depth_images;
		std::vector<vk::image_view*> m_depth_views;
		
		std::vector<vk::framebuffer*> m_framebuffers;
		std::vector<vk::framebuffer*> m_depthbuffers;
		
		std::vector<vk::render_frame*> m_frames;
		std::vector<vk::render_frame*>::iterator m_frame_iterator;
		std::map<vk::render_frame*, vk::forward_renderer::frame_object*> m_frame_objects;
		std::vector<vk::forward_renderer::draw_object*> m_draws;
	};
}

#endif