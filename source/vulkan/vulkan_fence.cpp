
// vulkan_fence.cpp
//
// source file for the RAII wrapper of the VkFence
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_fence.hpp"
#include <limits>

vk::fence::fence(vk::device& t_device, VkFenceCreateFlags t_flags) : i_device_object{t_device} {
	
	VkFenceCreateInfo fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.pNext = nullptr;
	fence_info.flags = t_flags;
	
	VK_DEBUG
		( vkCreateFence
		, "Failed to create fence"
		, m_device.get_device(), &fence_info, nullptr, &m_fence);
}

vk::fence::~fence() {
	
	vkDestroyFence(m_device.get_device(), m_fence, nullptr);
}

VkFence vk::fence::get_fence() {
	
	return m_fence;
}

void vk::fence::wait(uint64_t t_timeout) {
	
	VK_DEBUG
		( vkWaitForFences
		, "Failed to wait for fence"
		, m_device.get_device(), 1, &m_fence, true, t_timeout);
}

void vk::fence::reset() {
	
	VK_DEBUG
		( vkResetFences
		, "Failed to reset fence"
		, m_device.get_device(), 1, &m_fence);
}

void vk::fence::wait_and_reset() {
	
	wait(std::numeric_limits<uint64_t>::max());
	reset();
}