
// vulkan_render_pass.cpp
//
// source file for the RAII wrrapper of the VkRenderPass
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_render_pass.hpp"
#include "vulkan_device.hpp"

vk::render_pass::render_pass
	( vk::device& t_device
	, const VkAttachmentDescription* t_color_attachments, uint32_t t_color_attachment_count
	, const VkSubpassDescription* t_subpasses, uint32_t t_subpass_count
	, const VkSubpassDependency* t_subpass_dependancies, uint32_t t_subpass_dependency_count) : m_device{t_device} {
	
	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.pNext = nullptr;
	render_pass_info.flags = 0;
	render_pass_info.attachmentCount = t_color_attachment_count;
	render_pass_info.pAttachments = t_color_attachments;
	render_pass_info.subpassCount = t_subpass_count;
	render_pass_info.pSubpasses = t_subpasses;
	render_pass_info.dependencyCount = t_subpass_dependency_count;
	render_pass_info.pDependencies = t_subpass_dependancies;
	
	VK_DEBUG
		( vkCreateRenderPass
		, "Failed to create render pass."
		, m_device.get_device(), &render_pass_info, nullptr, &m_render_pass)
}

vk::render_pass::~render_pass() {
	
	vkDestroyRenderPass(m_device.get_device(), m_render_pass, nullptr);
}

vk::device& vk::render_pass::get_device() {
	
	return m_device;
}

const vk::device& vk::render_pass::get_device() const {
	
	return m_device;
}

VkRenderPass vk::render_pass::get_render_pass() {
	
	return m_render_pass;
}