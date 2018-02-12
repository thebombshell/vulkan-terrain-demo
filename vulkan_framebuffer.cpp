
// vulkan_framebuffer.cpp
//
// source file for the RAII wrapper of the VkFramebuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_framebuffer.hpp"
#include "vulkan_device.hpp"

vk::framebuffer::framebuffer
	( vk::device& t_device, vk::render_pass& t_render_pass
	, const vk::image_view* t_attachments, uint32_t t_attachment_count
	, uint32_t t_width, uint32_t t_height, uint32_t t_layers)
	: m_device{t_device}, m_render_pass{t_render_pass} { 
	
	VkFramebufferCreateInfo framebuffer_info = {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.pNext = nullptr;
	framebuffer_info.flags = 0;
	framebuffer_info.renderPass = t_render_pass.get_render_pass();
	framebuffer_info.attachmentCount = t_attachment_count;
	framebuffer_info.pAttachments = t_attachments;
	framebuffer_info.width = t_width;
	framebuffer_info.height = t_height;
	framebuffer_info.layers = t_layers;
	
	VK_DEBUG
		( vkCreateFramebuffer
		, "Failed to create framebuffer"
		, m_device.get_device(), &framebuffer_info, nullptr, &m_framebuffer)
	
}

vk::framebuffer::~framebuffer() {
	
	vkDestroyFramebuffer(m_device.get_device(), m_framebuffer, nullptr);
}

vk::device& vk::framebuffer::get_device() {
	
	return m_device;
}

const vk::device& vk::framebuffer::get_device() const {
	
	return m_device;
}

vk::render_pass& vk::framebuffer::get_render_pass() {
	
	return m_render_pass;
}

const vk::render_pass& vk::framebuffer::get_render_pass() const {
	
	return m_render_pass;
}

VkFramebuffer vk::framebuffer::get_framebuffer() {
	
	return m_framebuffer;
}