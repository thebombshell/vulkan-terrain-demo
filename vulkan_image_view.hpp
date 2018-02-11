
// vulkan_image_view.hpp
//
// header file for the RAII wrapper of the VkImageView
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_IMAGE_VIEW_HPP
#define VKCPP_VULKAN_IMAGE_VIEW_HPP

#include "vulkan.hpp"

namespace vk {
	
	class image_view {
		
		public:
		
		image_view(vk::device& t_device, vk::i_image& t_image, VkFormat t_format);
		~image_view();
		
		vk::i_image& get_image();
		const vk::i_image& get_image() const;
		VkImageView get_image_view() const;
		
		private:
		
		vk::device& m_device;
		vk::i_image& m_image;
		
		VkImageView m_image_view;
	};
}

#endif