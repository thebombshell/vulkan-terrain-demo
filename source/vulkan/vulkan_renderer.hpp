
// vulkan_renderer.hpp
//
// source file for the VKCPP renderer helper classes
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class render_frame : public i_device_object {
		
		public:
		
		render_frame(vk::swapchain& t_swapchain, vk::command_pool& t_pool);
		~render_frame();
		
		uint32_t get_image_index();
		void submit();
		
		vk::command_buffer& get_command_buffer();
		const vk::command_buffer& get_command_buffer() const;
		
		private:
		
		vk::swapchain& m_swapchain;
		
		vk::command_buffer* m_graphics_buffer;
		vk::semaphore* m_image_available_semaphore;
		vk::semaphore* m_render_complete_semaphore;
		vk::fence* m_ready_fence;
		uint32_t m_image_index;
	};
}