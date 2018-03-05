
// vulkan_semaphore.cpp
//
// source file for the RAII wrapper of the VkSemaphore
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_semaphore.hpp"

vk::semaphore::semaphore(vk::device& t_device) : i_device_object{t_device} {
	
	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_info.pNext = nullptr;
	semaphore_info.flags = 0;
	VK_DEBUG
		( vkCreateSemaphore
		, "Failed to create semaphore"
		, m_device.get_device(), &semaphore_info, nullptr, &m_semaphore)
	
}

vk::semaphore::~semaphore() {
	
	vkDestroySemaphore(m_device.get_device(), m_semaphore, nullptr);
}

VkSemaphore vk::semaphore::get_semaphore() {
	
	return m_semaphore;
}