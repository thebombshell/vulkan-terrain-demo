
// vulkan_descriptor_set_layout.cpp
//
// source file for the RAII wrrapper of the VkDescriptorSetLayout
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_descriptor_set_layout.hpp"

//
// descriptor_set_layout
//

vk::descriptor_set_layout::descriptor_set_layout(vk::device& t_device, std::vector<VkDescriptorSetLayoutBinding>& t_bindings) 
	: i_device_object{t_device} {
	
	VkDescriptorSetLayoutCreateInfo descriptor_set_info = {};
    descriptor_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_info.pNext = nullptr;
    descriptor_set_info.flags = 0;
    descriptor_set_info.bindingCount = static_cast<uint32_t>(t_bindings.size());
    descriptor_set_info.pBindings = &t_bindings[0];
	VK_DEBUG
		( vkCreateDescriptorSetLayout
		, "Failed to create descriptor set layout"
		, m_device.get_device(), &descriptor_set_info, nullptr, &m_descriptor_set_layout)
}

vk::descriptor_set_layout::~descriptor_set_layout() {
	
	vkDestroyDescriptorSetLayout(m_device.get_device(), m_descriptor_set_layout, nullptr);
}	

VkDescriptorSetLayout vk::descriptor_set_layout::get_descriptor_set_layout() {
	
	return m_descriptor_set_layout;
}
