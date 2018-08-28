
// vulkan_command_buffer.hpp
//
// header file for the RAII wrrapper of the VkCommandBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_COMMAND_BUFFER_HPP
#define VKCPP_VULKAN_COMMAND_BUFFER_HPP

#include "vulkan.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class command_buffer : public i_device_object {
		
		public:
		
		command_buffer(vk::command_pool& t_command_pool) ;
		~command_buffer();
		
		VkCommandBuffer get_command_buffer();
		
		void reset();
		void begin();
		void begin_render_pass
			( vk::render_pass& t_render_pass, vk::framebuffer& t_framebuffer
			, VkRect2D t_render_area, const VkClearValue* const t_clear_values, uint32_t t_clear_value_count);
		void bind_pipeline(vk::i_pipeline& t_pipeline);
		void bind_vertex_buffers(std::vector<vk::i_buffer*>& t_buffers);
		void bind_index_buffer(vk::i_buffer& t_buffer, VkIndexType t_index_type);
		void copy_staged_buffer(vk::staged_buffer& t_buffer, uint32_t t_size, uint32_t t_source_offset = 0, uint32_t t_destination_offset = 0);
		void draw(uint32_t t_vertex_count, uint32_t t_instance_count = 1, uint32_t t_vertex_offset = 0, uint32_t t_instance_offset = 0);
		void draw_indexed(uint32_t t_index_count, uint32_t t_instance_count = 1, uint32_t t_index_offset = 0, uint32_t t_vertex_offset = 0, uint32_t t_instance_offset = 0);
		void end_render_pass();
		void end();
		
		private:
		
		vk::command_pool& m_command_pool;
		
		VkCommandBuffer m_command_buffer;
	};
}

#endif