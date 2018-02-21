
// vulkan_device.cpp
//
// source file for the RAII wrapper of the VkPhysicalDevice and VkDevice
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_device.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_surface.hpp"

#include <algorithm>
#include <assert.h>
#include <cstring>
#include <stdexcept>

vk::device::device
	( vk::physical_device& t_physical_device
	, vk::surface& t_surface
	, const char* const* t_layer_names, uint32_t t_layer_count
	, const char* const* t_extension_names, uint32_t t_extension_count
	) : m_physical_device{t_physical_device}, m_surface{t_surface}, m_graphical_queue_family_index{-1}, m_present_queue_family_index{-1} {
	
	int i = -1;
	for (const VkQueueFamilyProperties& t_queue_family_properties : m_physical_device.get_queue_family_properties()) {
		
		++i;
		if (t_queue_family_properties.queueCount <= 0) {
			
			continue;
		}
		
		if (t_queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			
			m_graphical_queue_family_index = i;
		}
		if (m_surface.is_surface_supported_by_queue_family(i)) {
			
			m_present_queue_family_index = i;
		}
		if (m_graphical_queue_family_index >= 0 && m_present_queue_family_index >= 0) {
			
			break;
		}
	}
	
	if (m_graphical_queue_family_index < 0) {
		
		throw vk::vulkan_exception("Failed to find suitable graphical queue family.");
	}
	if (m_present_queue_family_index < 0) {
		
		throw vk::vulkan_exception("Failed to find suitable present queue family.");
	}
	
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::vector<float> queue_priorities = {1.0f};
	VkDeviceQueueCreateInfo graphical_queue_create_info = {};
	graphical_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphical_queue_create_info.pNext = nullptr;
	graphical_queue_create_info.flags = 0;
	graphical_queue_create_info.queueFamilyIndex = m_graphical_queue_family_index;
	graphical_queue_create_info.queueCount = 1;
	graphical_queue_create_info.pQueuePriorities = queue_priorities.data();
	queue_create_infos.push_back(graphical_queue_create_info);
	if (m_graphical_queue_family_index != m_present_queue_family_index) {
		
		VkDeviceQueueCreateInfo present_queue_create_info = {};
		present_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		present_queue_create_info.pNext = nullptr;
		present_queue_create_info.flags = 0;
		present_queue_create_info.queueFamilyIndex = m_present_queue_family_index;
		present_queue_create_info.queueCount = 1;
		present_queue_create_info.pQueuePriorities = queue_priorities.data();
		queue_create_infos.push_back(present_queue_create_info);
	}
	
	VkPhysicalDeviceFeatures device_features = {};
	
	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext = nullptr;
	device_create_info.flags = 0;
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.enabledLayerCount = t_layer_count;
	device_create_info.ppEnabledLayerNames = t_layer_names;
	device_create_info.enabledExtensionCount = t_extension_count;
	device_create_info.ppEnabledExtensionNames = t_extension_names;
	device_create_info.pEnabledFeatures = &device_features;
	
	VK_DEBUG
		( vkCreateDevice
		, "Failed to create logical device"
		, m_physical_device.get_physical_device(), &device_create_info, nullptr, &m_device)
	
	vkGetDeviceQueue(m_device, m_graphical_queue_family_index, 0, &m_graphical_queue);
	vkGetDeviceQueue(m_device, m_present_queue_family_index, 0, &m_present_queue);
}

vk::device::~device() {
	
	vkDestroyDevice(m_device, nullptr);
}

vk::physical_device& vk::device::get_physical_device() {
	
	return m_physical_device;
}

const vk::physical_device& vk::device::get_physical_device() const {
	
	return m_physical_device;
}

VkDevice vk::device::get_device() const {
	
	return m_device;
}

VkQueue vk::device::get_graphical_queue() const {
	
	return m_graphical_queue;
}

VkQueue vk::device::get_present_queue() const {
	
	return m_present_queue;
}

int vk::device::get_graphical_queue_family_index() const {
	
	return m_graphical_queue_family_index;
}

int vk::device::get_present_queue_family_index() const {
	
	return m_present_queue_family_index;
}

void vk::device::submit_graphical_queue( const VkSubmitInfo* t_submit_infos, uint32_t t_submit_info_count, vk::fence* t_fence) {
	
	VK_DEBUG
		( vkQueueSubmit
		, "Failed to submit queue"
		, m_graphical_queue, t_submit_info_count, t_submit_infos, (t_fence ? t_fence->get_fence() : VK_NULL_HANDLE))
}

void vk::device::queue_pressent(std::vector<VkSemaphore>& t_semaphores, std::vector<VkSwapchainKHR>& t_swapchains, std::vector<uint32_t>& t_image_indices) {
	
	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext = nullptr;
	present_info.waitSemaphoreCount = static_cast<uint32_t>(t_semaphores.size());
	present_info.pWaitSemaphores = t_semaphores.data();
	present_info.swapchainCount = static_cast<uint32_t>(t_swapchains.size());
	present_info.pSwapchains = t_swapchains.data();
	present_info.pImageIndices = t_image_indices.data();
	present_info.pResults = nullptr;
	
	VK_DEBUG
		( vkQueuePresentKHR
		, "Failed to present queue"
		, m_present_queue, &present_info)
}

vk::device_object::device_object(vk::device& t_device) : m_device{t_device} {
	
}

vk::device_object::device_object(vk::device_object& t_device_object) : m_device{t_device_object.m_device} {
	
}

vk::device_object::~device_object() {
	
}

vk::device& vk::device_object::get_device() {
	
	return m_device;
}

const vk::device& vk::device_object::get_device() const {
	
	return m_device;
}