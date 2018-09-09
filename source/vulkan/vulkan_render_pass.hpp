
// vulkan_render_pass.cpp
//
// source file for the RAII wrrapper of the VkRenderPass
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_RENDER_PASS_HPP
#define VKCPP_VULKAN_RENDER_PASS_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	VkAttachmentDescription get_present_attachment_description(VkFormat t_format);
	
	VkAttachmentDescription get_depth_stencil_attachment_description(VkFormat t_format);
	
	VkSubpassDescription get_graphics_subpass_description
		( uint32_t t_in_attachment_count = 0, const VkAttachmentReference* t_in_attachments = nullptr
		, uint32_t t_out_color_count = 0, const VkAttachmentReference* t_out_color_attachments = nullptr
		, const VkAttachmentReference* t_out_resolve_attachments = nullptr
		, const VkAttachmentReference* t_out_depth_stencil_attachments = nullptr
		, uint32_t t_preserve_count = 0, const uint32_t* t_preserve_attachments = nullptr);
	
	VkSubpassDescription get_compute_subpass_description
		( std::vector<VkAttachmentReference>* t_in_attachments = nullptr
		, std::vector<VkAttachmentReference>* t_out_color_attachments = nullptr
		, std::vector<VkAttachmentReference>* t_out_resolve_attachments = nullptr
		, std::vector<VkAttachmentReference>* t_out_depth_stencil_attachments = nullptr
		, std::vector<uint32_t>* t_preserve_attachments = nullptr);
	
	VkSubpassDependency get_single_subpass_dependancy
		( VkPipelineStageFlags t_source_stage = 0
		, VkPipelineStageFlags t_destination_stage = 0
		, VkAccessFlags t_source_access = 0
		, VkAccessFlags t_destination_access = 0
		, VkDependencyFlags t_dependancies = 0);
		
	VkSubpassDependency get_external_subpass_dependancy
		( VkPipelineStageFlags t_source_stage = 0
		, VkPipelineStageFlags t_destination_stage = 0
		, VkAccessFlags t_source_access = 0
		, VkAccessFlags t_destination_access = 0
		, VkDependencyFlags t_dependancies = 0);
	
	class render_pass : public i_device_object {
	
	public:
	
	render_pass
		( vk::device& t_device
		, const VkAttachmentDescription* t_color_attachments = nullptr, uint32_t t_color_attachment_count = 0
		, const VkSubpassDescription* t_subpasses = nullptr, uint32_t t_subpass_count = 0
		, const VkSubpassDependency* t_subpass_dependancies = nullptr, uint32_t t_subpass_dependency_count = 0);
	~render_pass();
	
	VkRenderPass get_render_pass();
	
	private:
	
	VkRenderPass m_render_pass;
	};
}
#endif