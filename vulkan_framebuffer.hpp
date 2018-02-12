
// vulkan_framebuffer.hpp
//
// header file for the RAII wrapper of the VkFramebuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_FRAMEBUFFER_HPP
#define VKCPP_VULKAN_FRAMEBUFFER_HPP

#include "vulkan.hpp"

namespace vk {
	
	class framebuffer {
		
		public:
		
		framebuffer(vk::device& t_device, vk::graphics_pipeline& t_pipeline);
		~framebuffer();
		
		vk::device& get_device();
		const vk::device& get_device() const;
		
		vk::graphics_pipeline& get_pipeline();
		const vk::graphics_pipeline& get_pipeline() const;
		
		VkFramebuffer get_framebuffer();
		
		private:
		
		vk::device& m_device;
		vk::graphics_pipeline& m_pipeline;
		
		VkFramebuffer m_framebuffer;
	};
}

#endif