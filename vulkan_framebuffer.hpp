
// vulkan_framebuffer.hpp
//
// header file for the RAII wrapper of the VkFramebuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_FRAMEBUFFER_HPP
#define VKCPP_VULKAN_FRAMEBUFFER_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class framebuffer : public device_object {
		
		public:
		
		framebuffer
			( vk::device& t_device, vk::render_pass& t_render_pass
			, std::vector<vk::image_view*>& t_attachments
			, uint32_t t_width, uint32_t t_height, uint32_t t_layers);
		~framebuffer();
		
		vk::render_pass& get_render_pass();
		const vk::render_pass& get_render_pass() const;
		
		VkFramebuffer get_framebuffer();
		
		private:
		
		vk::render_pass& m_render_pass;
		
		VkFramebuffer m_framebuffer;
	};
}

#endif