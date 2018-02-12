
// vulkan_render_pass.cpp
//
// source file for the RAII wrrapper of the VkRenderPass
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_RENDER_PASS_HPP
#define VKCPP_VULKAN_RENDER_PASS_HPP

#include "vulkan.hpp"

namespace vk {
	
	class render_pass{
	
	public:
	
	render_pass
		( vk::device& t_device
		, const VkAttachmentDescription* t_color_attachments = nullptr, uint32_t t_color_attachment_count = 0
		, const VkSubpassDescription* t_subpasses = nullptr, uint32_t t_subpass_count = 0
		, const VkSubpassDependency* t_subpass_dependancies = nullptr, uint32_t t_subpass_dependency_count = 0);
	~render_pass();
	
	vk::device& get_device();
	const vk::device& get_device() const;
	
	VkRenderPass get_render_pass();
	
	private:
	
	vk::device& m_device;
	
	VkRenderPass m_render_pass;
	};
}
#endif