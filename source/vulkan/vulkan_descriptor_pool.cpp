
// vulkan_descriptor_pool.cpp
//
// source file for the RAII wrapper of the VkDescriptorPool
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_descriptor_pool.hpp"

vk::descriptor_pool::descriptor_pool(vk::device& t_device, uint32_t t_max_sets, std::vector<VkDescriptorPoolSize >& t_pool_sizes) 
	: i_device_object{t_device} {

	VkDescriptorPoolCreateInfo descriptor_pool_info = {};
	descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptor_pool_info.pNext = nullptr;
	descriptor_pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptor_pool_info.maxSets = t_max_sets;
	descriptor_pool_info.poolSizeCount = static_cast<uint32_t>(t_pool_sizes.size());
	descriptor_pool_info.pPoolSizes = &t_pool_sizes[0];
	
	VK_DEBUG( vkCreateDescriptorPool
		, "failed to create descriptor pool"
		, m_device.get_device(), &descriptor_pool_info, nullptr, &m_descriptor_pool );
}

vk::descriptor_pool::~descriptor_pool() {
	
	vkDestroyDescriptorPool(m_device.get_device(), m_descriptor_pool, nullptr);
}

VkDescriptorPool vk::descriptor_pool::get_descriptor_pool() {
	
	return m_descriptor_pool;
}