
// vulkan_command_buffer.hpp
//
// header file for the RAII wrrapper of the VkCommandBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_COMMAND_BUFFER_HPP
#define VKCPP_VULKAN_COMMAND_BUFFER_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class command_buffer : public device_object {
		
		public:
		
		command_buffer(vk::device& t_device, vk::command_pool& t_command_pool) ;
		~command_buffer();
		
		VkCommandBuffer get_command_buffer();
		
		void begin();
		void begin_render_pass
			( vk::render_pass& t_render_pass, vk::framebuffer& t_framebuffer
			, VkRect2D t_render_area, const VkClearValue* const t_clear_values, uint32_t t_clear_value_count);
		void bind_pipeline(vk::pipeline& t_pipeline);
		void bind_buffers(std::vector<vk::buffer*>& t_buffers);
		void draw(uint32_t t_vertex_count, uint32_t t_instance_count = 1, uint32_t t_vertex_offset = 0, uint32_t t_instance_offset = 0);
		void end_render_pass();
		void end();
		
		private:
		
		vk::command_pool& m_command_pool;
		
		VkCommandBuffer m_command_buffer;
	};
}

#endif