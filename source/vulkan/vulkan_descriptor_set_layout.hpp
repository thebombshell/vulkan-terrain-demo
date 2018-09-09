
// vulkan_descriptor_set_layout.hpp
//
// header file for the RAII wrapper of the VkDescriptorSetLayout
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_DESCRIPTOR_SET_LAYOUT_HPP
#define VKCPP_VULKAN_DESCRIPTOR_SET_LAYOUT_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class descriptor_set_layout : public i_device_object {
		
		public:
		
		descriptor_set_layout(vk::device& t_device, std::vector<VkDescriptorSetLayoutBinding>& t_bindings);
		~descriptor_set_layout();
		
		VkDescriptorSetLayout get_descriptor_set_layout();
		
		private:
		
		VkDescriptorSetLayout m_descriptor_set_layout;
	};
}

#endif