
// vulkan_buffer.cpp
//
// source file for the RAII wrapper of the VkBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_buffer.hpp"
#include "vulkan_physical_device.hpp"

vk::buffer::buffer(vk::device& t_device, uint32_t t_size, VkBufferUsageFlags t_usage, VkSharingMode t_sharing_mode) 
	: device_object{t_device} {
	
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext = nullptr;
	buffer_info.flags = 0;
	buffer_info.size = t_size;
	buffer_info.usage = t_usage;
	buffer_info.sharingMode = t_sharing_mode;
	buffer_info.queueFamilyIndexCount = 0;
	buffer_info.pQueueFamilyIndices = nullptr;
	
	VK_DEBUG
		( vkCreateBuffer
		, "Failed to create buffer"
		, m_device.get_device(), &buffer_info, nullptr, &m_buffer)
	
	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(m_device.get_device(), m_buffer, &memory_requirements);
	
	VkMemoryAllocateInfo memory_alocate_info = {};
	memory_alocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_alocate_info.pNext = nullptr;
	memory_alocate_info.allocationSize = memory_requirements.size;
	memory_alocate_info.memoryTypeIndex = m_device.get_physical_device().find_memory_index
		( memory_requirements.memoryTypeBits
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	VK_DEBUG
		( vkAllocateMemory
		, "Failed to allocate memory"
		, m_device.get_device(), &memory_alocate_info, nullptr, &m_device_memory)
	
	VK_DEBUG
		( vkBindBufferMemory
		, "Failed to bind buffer memory"
		, m_device.get_device(), m_buffer, m_device_memory, 0)
}

vk::buffer::~buffer() {
	
	vkFreeMemory(m_device.get_device(), m_device_memory, nullptr);
	vkDestroyBuffer(m_device.get_device(), m_buffer, nullptr);
}

void vk::buffer::map(const void* t_data_pointer, uint32_t t_data_size) {
	
	void* pointer = nullptr;
	VK_DEBUG
		( vkMapMemory
		, "Failed to map memory"
		, m_device.get_device(), m_device_memory, 0, t_data_size, 0, &pointer)
	memcpy(pointer, t_data_pointer, static_cast<size_t>(t_data_size));
	vkUnmapMemory(m_device.get_device(), m_device_memory);
}

VkBuffer vk::buffer::get_buffer() {
	
	return m_buffer;
}

vk::staged_buffer::staged_buffer(vk::device& t_device, uint32_t t_size, VkBufferUsageFlags t_usage) 
	: buffer{t_device, t_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE} {
	
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext = nullptr;
	buffer_info.flags = 0;
	buffer_info.size = t_size;
	buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | t_usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	buffer_info.queueFamilyIndexCount = 0;
	buffer_info.pQueueFamilyIndices = nullptr;
	
	VK_DEBUG
		( vkCreateBuffer
		, "Failed to create buffer"
		, m_device.get_device(), &buffer_info, nullptr, &m_destination_buffer)
	
	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(m_device.get_device(), m_destination_buffer, &memory_requirements);
	
	VkMemoryAllocateInfo memory_alocate_info = {};
	memory_alocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_alocate_info.pNext = nullptr;
	memory_alocate_info.allocationSize = memory_requirements.size;
	memory_alocate_info.memoryTypeIndex = m_device.get_physical_device().find_memory_index
		( memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
	VK_DEBUG
		( vkAllocateMemory
		, "Failed to allocate memory"
		, m_device.get_device(), &memory_alocate_info, nullptr, &m_destination_device_memory)
	
	VK_DEBUG
		( vkBindBufferMemory
		, "Failed to bind buffer memory"
		, m_device.get_device(), m_destination_buffer, m_destination_device_memory, 0)
	
}

vk::staged_buffer::~staged_buffer() {
	
	vkFreeMemory(m_device.get_device(), m_destination_device_memory, nullptr);
	vkDestroyBuffer(m_device.get_device(), m_destination_buffer, nullptr);
}

VkBuffer vk::staged_buffer::get_buffer() {
	
	return m_destination_buffer;
}

VkBuffer vk::staged_buffer::get_staging_buffer() {
	
	return m_buffer;
}

vk::vertex_buffer::vertex_buffer(vk::device& t_device, uint32_t t_size) 
	: buffer{t_device, t_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE} {
	
}

vk::vertex_buffer::~vertex_buffer() {
	
	
}