
// vulkan_framebuffer.cpp
//
// source file for the RAII wrapper of the VkFramebuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_framebuffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_image_view.hpp"
#include "vulkan_render_pass.hpp"

vk::framebuffer::framebuffer
	( vk::device& t_device, vk::render_pass& t_render_pass
	, std::vector<vk::image_view*>& t_attachments
	, VkExtent2D t_resolution, uint32_t t_layers)
	: device_object{t_device}, m_render_pass{t_render_pass} { 
	
	std::vector<VkImageView> attachments{t_attachments.size()};
	int i = 0;
	for (auto* t_image_view : t_attachments) {
		
		attachments[i++] = t_image_view->get_image_view();
	}
	
	VkFramebufferCreateInfo framebuffer_info = {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.pNext = nullptr;
	framebuffer_info.flags = 0;
	framebuffer_info.renderPass = t_render_pass.get_render_pass();
	framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebuffer_info.pAttachments = attachments.data();
	framebuffer_info.width = t_resolution.width;
	framebuffer_info.height = t_resolution.height;
	framebuffer_info.layers = t_layers;
	
	VK_DEBUG
		( vkCreateFramebuffer
		, "Failed to create framebuffer"
		, m_device.get_device(), &framebuffer_info, nullptr, &m_framebuffer)
}

vk::framebuffer::~framebuffer() {
	
	vkDestroyFramebuffer(m_device.get_device(), m_framebuffer, nullptr);
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