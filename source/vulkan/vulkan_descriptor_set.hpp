
// vulkan_descriptor_set.hpp
//
// header file for the RAII wrapper of the VkDescriptorSet
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_DESCRIPTOR_SET_HPP
#define VKCPP_VULKAN_DESCRIPTOR_SET_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class descriptor_set : public i_device_object {
		
		public:
		
		descriptor_set(vk::descriptor_pool& t_descriptor_pool, vk::descriptor_set_layout& t_layout);
		~descriptor_set();
		
		VkDescriptorSet get_descriptor_set();
		
		void write(uint32_t t_binding, VkDescriptorType t_type, uint32_t t_offset = 0, uint32_t t_count = 1 
			, const VkDescriptorImageInfo* t_image_info = nullptr
			, const VkDescriptorBufferInfo* t_buffer_info = nullptr
			, const VkBufferView* t_texel_info = nullptr);
		
		private:
		
		VkDescriptorSet m_descriptor_set;
		vk::descriptor_pool& m_descriptor_pool;
	};
}

#endif