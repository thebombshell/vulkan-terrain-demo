
// vulkan_command_pool.cpp
//
// source file for the RAII wrrapper of the VkCommandPool
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_command_pool.hpp"

vk::command_pool::command_pool(vk::device& t_device, uint32_t t_queue_family_index) : i_device_object{t_device} {
	
	VkCommandPoolCreateInfo command_pool_info = {};
	command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_info.pNext = nullptr;
	command_pool_info.flags = 0;
	command_pool_info.queueFamilyIndex = t_queue_family_index;
	
	VK_DEBUG
		( vkCreateCommandPool
		, "Failed to create command pool"
		, m_device.get_device(), &command_pool_info, nullptr, &m_command_pool)
}

vk::command_pool::~command_pool() {
	
	vkDestroyCommandPool(m_device.get_device(), m_command_pool, nullptr);
}

VkCommandPool vk::command_pool::get_command_pool() {
	
	return m_command_pool;
}