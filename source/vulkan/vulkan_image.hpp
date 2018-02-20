
// vulkan_image.hpp
//
// header file for the RAII wrapper of the VkImage
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_IMAGE_HPP
#define VKCPP_VULKAN_IMAGE_HPP

#include "vulkan.hpp"

namespace vk {
	
	class i_image {
		
		public:
		
		i_image();
		virtual ~i_image() = 0;
		
		virtual VkImage get_image() const = 0;
		
		private:
		
	};
	
	class image_reference : public i_image {
		
		public:
		
		image_reference(VkImage t_image);
		~image_reference();
		
		VkImage get_image() const override;
		
		private:
		
		VkImage m_image;
	};
	
	class image : public i_image {
		
		public:
		
		image();
		~image();
		
		VkImage get_image() const override;
		
		private:
		
		VkImage m_image;
	};
}

#endif