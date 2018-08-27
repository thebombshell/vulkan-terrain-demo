
// vulkan_render_pass.cpp
//
// source file for the RAII wrrapper of the VkRenderPass
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_render_pass.hpp"
#include "vulkan_device.hpp"

VkAttachmentDescription vk::get_present_attachment_description(VkFormat t_format) {
	
	return 
		{ 0
		, t_format
		, VK_SAMPLE_COUNT_1_BIT
		, VK_ATTACHMENT_LOAD_OP_CLEAR
		, VK_ATTACHMENT_STORE_OP_STORE
		, VK_ATTACHMENT_LOAD_OP_DONT_CARE
		, VK_ATTACHMENT_STORE_OP_DONT_CARE
		, VK_IMAGE_LAYOUT_UNDEFINED
		, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};
}

VkSubpassDescription vk::get_graphics_subpass_description
	( std::vector<VkAttachmentReference>* t_in_attachments
	, std::vector<VkAttachmentReference>* t_out_color_attachments
	, std::vector<VkAttachmentReference>* t_out_resolve_attachments
	, std::vector<VkAttachmentReference>* t_out_depth_stencil_attachments
	, std::vector<uint32_t>* t_preserve_attachments) {
	
	return 
		{ 0
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, t_in_attachments ? static_cast<uint32_t>(t_in_attachments->size()) : 0
		, t_in_attachments ? &(*t_in_attachments)[0] : nullptr
		, t_out_color_attachments ? static_cast<uint32_t>(t_out_color_attachments->size()) : 0
		, t_out_color_attachments ? &(*t_out_color_attachments)[0] : nullptr
		, t_out_resolve_attachments ? &(*t_out_resolve_attachments)[0] : nullptr
		, t_out_depth_stencil_attachments ? &(*t_out_depth_stencil_attachments)[0] : nullptr
		, t_preserve_attachments ? static_cast<uint32_t>(t_preserve_attachments->size()) : 0
		, t_preserve_attachments ? &(*t_preserve_attachments)[0] : nullptr
		};
}
VkSubpassDescription vk::get_compute_subpass_description
	( std::vector<VkAttachmentReference>* t_in_attachments
	, std::vector<VkAttachmentReference>* t_out_color_attachments
	, std::vector<VkAttachmentReference>* t_out_resolve_attachments
	, std::vector<VkAttachmentReference>* t_out_depth_stencil_attachments
	, std::vector<uint32_t>* t_preserve_attachments) {
	
	return 
		{ 0
		, VK_PIPELINE_BIND_POINT_COMPUTE
		, t_in_attachments ? static_cast<uint32_t>(t_in_attachments->size()) : 0
		, t_in_attachments ? &(*t_in_attachments)[0] : nullptr
		, t_out_color_attachments ? static_cast<uint32_t>(t_out_color_attachments->size()) : 0
		, t_out_color_attachments ? &(*t_out_color_attachments)[0] : nullptr
		, t_out_resolve_attachments ? &(*t_out_resolve_attachments)[0] : nullptr
		, t_out_depth_stencil_attachments ? &(*t_out_depth_stencil_attachments)[0] : nullptr
		, t_preserve_attachments ? static_cast<uint32_t>(t_preserve_attachments->size()) : 0
		, t_preserve_attachments ? &(*t_preserve_attachments)[0] : nullptr
		};
}

VkSubpassDependency vk::get_single_subpass_dependancy
	( VkPipelineStageFlags t_source_stage
	, VkPipelineStageFlags t_destination_stage
	, VkAccessFlags t_source_access
	, VkAccessFlags t_destination_access
	, VkDependencyFlags t_dependancies
	) {
	
	return 
		{ 0
		, 0
		, t_source_stage
		, t_destination_stage
		, t_source_access
		, t_destination_access
		, t_dependancies
		};
}

VkSubpassDependency vk::get_external_subpass_dependancy
	( VkPipelineStageFlags t_source_stage
	, VkPipelineStageFlags t_destination_stage
	, VkAccessFlags t_source_access
	, VkAccessFlags t_destination_access
	, VkDependencyFlags t_dependancies
	) {
	
	return 
		{ VK_SUBPASS_EXTERNAL
		, 0
		, t_source_stage
		, t_destination_stage
		, t_source_access
		, t_destination_access
		, t_dependancies
		};
}

vk::render_pass::render_pass
	( vk::device& t_device
	, const VkAttachmentDescription* t_color_attachments, uint32_t t_color_attachment_count
	, const VkSubpassDescription* t_subpasses, uint32_t t_subpass_count
	, const VkSubpassDependency* t_subpass_dependancies, uint32_t t_subpass_dependency_count) : i_device_object{t_device} {
	
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

VkRenderPass vk::render_pass::get_render_pass() {
	
	return m_render_pass;
}