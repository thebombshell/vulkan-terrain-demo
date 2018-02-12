
// vulkan_framebuffer.cpp
//
// source file for the RAII wrapper of the VkFramebuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_framebuffer.hpp"
#include "vulkan_device.hpp"

vk::framebuffer::framebuffer(vk::device& t_device, vk::graphics_pipeline& t_pipeline) 
	: m_device{t_device}, m_pipeline{t_pipeline} { 
	
	// VkFramebufferCreateInfo framebuffer_info = {};
	// framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	// framebuffer_info.pNext = nullptr;
	// framebuffer_info.flags = 0;
	// framebuffer_info.renderPass;
	// framebuffer_info.attachmentCount;
	// framebuffer_info.pAttachments;
	// framebuffer_info.width;
	// framebuffer_info.height;
	// framebuffer_info.layers = 1;
	
}

vk::framebuffer::~framebuffer() {
	
}

vk::device& vk::framebuffer::get_device() {
	
	return m_device;
}

const vk::device& vk::framebuffer::get_device() const {
	
	return m_device;
}

vk::graphics_pipeline& vk::framebuffer::get_pipeline() {
	
	return m_pipeline;
}

const vk::graphics_pipeline& vk::framebuffer::get_pipeline() const {
	
	return m_pipeline;
}

VkFramebuffer vk::framebuffer::get_framebuffer() {
	
	return m_framebuffer;
}