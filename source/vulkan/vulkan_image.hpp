
// vulkan_image.hpp
//
// header file for the RAII wrapper of the VkImage
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_IMAGE_HPP
#define VKCPP_VULKAN_IMAGE_HPP

#include "vulkan.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	const std::vector<VkFormat>& get_depth_formats();
	
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
	
	class image : public i_image, public i_device_object {
		
		public:
		
		image(vk::device& t_device, VkImageType t_type, VkFormat t_format, VkExtent3D t_extent, VkImageCreateFlags t_flags
			, uint32_t t_mip_levels, uint32_t t_array_layers, VkSampleCountFlagBits t_sample_count, VkImageTiling t_tiling
			, VkImageLayout t_layout, VkImageUsageFlags t_usage, VkSharingMode t_sharing_mode, std::vector<uint32_t>* t_queues);
		~image();
		
		VkImage get_image() const override;
		
		private:
		
		VkImage m_image;
		VkDeviceMemory m_device_memory;
	};
	
	class depth_image : public image {
		
		public:
		
		depth_image(vk::device& t_device, VkExtent3D t_extent);
		depth_image(vk::device& t_device, VkFormat t_format, VkExtent3D t_extent);
		~depth_image();
		
		private:
	};
}

#endif