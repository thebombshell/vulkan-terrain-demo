
// vulkan_descriptor_set.cpp
//
// source file for the RAII wrrapper of the VkDescriptorSetLayout
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_descriptor_pool.hpp"
#include "vulkan_descriptor_set.hpp"
#include "vulkan_descriptor_set_layout.hpp"

//
// descriptor_set
//

vk::descriptor_set::descriptor_set(vk::descriptor_pool& t_descriptor_pool, vk::descriptor_set_layout& t_layout) 
	: i_device_object{t_descriptor_pool}, m_descriptor_pool{t_descriptor_pool} {
	
	VkDescriptorSetLayout layout = t_layout.get_descriptor_set_layout();
	VkDescriptorSetAllocateInfo descriptor_set_info = {};
    descriptor_set_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_set_info.pNext = 0;
    descriptor_set_info.descriptorPool = m_descriptor_pool.get_descriptor_pool();
    descriptor_set_info.descriptorSetCount = 1;
    descriptor_set_info.pSetLayouts = &layout;
	VK_DEBUG
		( vkAllocateDescriptorSets
		, "Failed to create descriptor set"
		, m_device.get_device(), &descriptor_set_info, &m_descriptor_set)
}

vk::descriptor_set::~descriptor_set() {
	
	vkFreeDescriptorSets(m_device.get_device(), m_descriptor_pool.get_descriptor_pool(), 1, &m_descriptor_set);
}	

VkDescriptorSet vk::descriptor_set::get_descriptor_set() {
	
	return m_descriptor_set;
}

void vk::descriptor_set::write(uint32_t t_binding, VkDescriptorType t_type, uint32_t t_offset, uint32_t t_count
    , const VkDescriptorImageInfo* t_image_info, const VkDescriptorBufferInfo* t_buffer_info, const VkBufferView* t_texel_info) {
	
	VkWriteDescriptorSet write_info = {};
	write_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write_info.pNext = 0;
	write_info.dstSet = m_descriptor_set;
	write_info.dstBinding = t_binding;
	write_info.dstArrayElement = t_offset;
	write_info.descriptorCount = t_count;
	write_info.descriptorType = t_type;
	write_info.pImageInfo = t_image_info;
	write_info.pBufferInfo = t_buffer_info;
	write_info.pTexelBufferView = t_texel_info;
	
	vkUpdateDescriptorSets(m_device.get_device(), 1, &write_info, 0, nullptr);
}
