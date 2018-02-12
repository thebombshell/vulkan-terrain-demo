
// vulkan_pipeline.cpp
//
// source file for the RAII wrrapper of the VkPipeline
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_pipeline.hpp"
#include "vulkan_device.hpp"
#include "vulkan_pipeline_layout.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

#include <fstream>

const std::vector<VkDynamicState> g_dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

vk::graphics_pipeline::graphics_pipeline(vk::device& t_device, vk::swapchain& t_swapchain)
	: m_device{t_device}, m_swapchain{t_swapchain}
	, m_vertex_shader_module{nullptr}, m_fragment_shader_module{nullptr}
	, m_pipeline_layout{nullptr}, m_render_pass{nullptr} {
	
	m_vertex_shader_module = new vk::shader_module(m_device, "vert.spv");
	m_fragment_shader_module = new vk::shader_module(m_device, "frag.spv");
	
	std::vector<VkPipelineShaderStageCreateInfo> shader_stage_info;
	VkVertexInputBindingDescription vertex_binding;
	std::vector<VkVertexInputAttributeDescription> vertex_attributes;
	VkPipelineVertexInputStateCreateInfo vertex_input_info;
	VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewport_info;
	VkPipelineRasterizationStateCreateInfo rasterization_info;
	VkPipelineMultisampleStateCreateInfo multisample_info;
	VkPipelineColorBlendAttachmentState color_blend_state;
	VkPipelineColorBlendStateCreateInfo color_blend_info;
	VkAttachmentDescription color_attachment;
	VkAttachmentReference color_attachment_reference;
	VkSubpassDescription sub_pass;
	
	shader_stage_info.resize(2);
	shader_stage_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info[0].pNext = nullptr;
	shader_stage_info[0].flags = 0;
	shader_stage_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shader_stage_info[0].module = m_vertex_shader_module->get_shader_module();
	shader_stage_info[0].pName = "main";
	shader_stage_info[0].pSpecializationInfo = nullptr;
	shader_stage_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info[1].pNext = nullptr;
	shader_stage_info[1].flags = 0;
	shader_stage_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shader_stage_info[1].module = m_fragment_shader_module->get_shader_module();
	shader_stage_info[1].pName = "main";
	shader_stage_info[1].pSpecializationInfo = nullptr;
	
	vertex_binding.binding = 0;
	vertex_binding.stride = sizeof(float) * 6;
	vertex_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertex_attributes.resize(2);
	vertex_attributes[0].location = 0;
	vertex_attributes[0].binding = 0;
	vertex_attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertex_attributes[0].offset = 0;
	vertex_attributes[1].location = 1;
	vertex_attributes[1].binding = 0;
	vertex_attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertex_attributes[1].offset = 0;
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.pNext = nullptr;
	vertex_input_info.flags = 0;
	vertex_input_info.vertexBindingDescriptionCount = 1;
	vertex_input_info.pVertexBindingDescriptions = &vertex_binding;
	vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attributes.size());
	vertex_input_info.pVertexAttributeDescriptions = vertex_attributes.data();
	
	input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_info.pNext = nullptr;
	input_assembly_info.flags = 0;
	input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	input_assembly_info.primitiveRestartEnable = VK_TRUE;
	
	VkExtent2D extent = m_swapchain.get_extent();
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0;
	viewport.maxDepth = 1.0;
	
	scissor.offset = {0, 0};
	scissor.extent = extent;
	
	viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_info.pNext = nullptr;
	viewport_info.flags = 0;
	viewport_info.viewportCount = 1;
	viewport_info.pViewports = &viewport;
	viewport_info.scissorCount = 1;
	viewport_info.pScissors = &scissor;
	
	rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterization_info.pNext = nullptr;
	rasterization_info.flags = 0;
	rasterization_info.depthClampEnable = VK_FALSE;
	rasterization_info.rasterizerDiscardEnable = VK_TRUE;
	rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterization_info.depthBiasEnable = VK_FALSE;
	rasterization_info.depthBiasConstantFactor = 0.0f;
	rasterization_info.depthBiasClamp = 0.0f;
	rasterization_info.depthBiasSlopeFactor = 0.0f;
	rasterization_info.lineWidth = 1.0f;
	
	multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample_info.pNext = nullptr;
	multisample_info.flags = 0;
	multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisample_info.sampleShadingEnable = VK_FALSE;
	multisample_info.minSampleShading = 1.0f;
	multisample_info.pSampleMask = nullptr;
	multisample_info.alphaToCoverageEnable = VK_FALSE;
	multisample_info.alphaToOneEnable = VK_FALSE;
	
	color_blend_state.blendEnable = VK_FALSE;
	color_blend_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_state.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_state.alphaBlendOp = VK_BLEND_OP_ADD;
	color_blend_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	
	color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_info.pNext = nullptr;
	color_blend_info.flags = 0;
	color_blend_info.logicOpEnable = VK_FALSE;
	color_blend_info.logicOp = VK_LOGIC_OP_COPY;
	color_blend_info.attachmentCount = 1;
	color_blend_info.pAttachments = &color_blend_state;
	color_blend_info.blendConstants[0] = 0.0f;
	color_blend_info.blendConstants[1] = 0.0f;
	color_blend_info.blendConstants[2] = 0.0f;
	color_blend_info.blendConstants[3] = 0.0f;
	
	color_attachment.flags = 0;
	color_attachment.format = m_swapchain.get_surface_format().format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	
	color_attachment_reference.attachment = 0;
	color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	sub_pass.flags = 0;
	sub_pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	sub_pass.inputAttachmentCount = 0;
	sub_pass.pInputAttachments = nullptr;
	sub_pass.colorAttachmentCount = 1;
	sub_pass.pColorAttachments = &color_attachment_reference;
	sub_pass.pResolveAttachments = nullptr;
	sub_pass.pDepthStencilAttachment = nullptr;
	sub_pass.preserveAttachmentCount = 0;
	sub_pass.pPreserveAttachments = nullptr;
	
	m_pipeline_layout = new vk::pipeline_layout(m_device, nullptr, 0, nullptr, 0);
	m_render_pass = new vk::render_pass(m_device, &color_attachment, 1, &sub_pass, 1);
	
	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pNext = nullptr;
	pipeline_info.flags = 0;
	pipeline_info.stageCount = static_cast<uint32_t>(shader_stage_info.size());
	pipeline_info.pStages = shader_stage_info.data();
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly_info;
	pipeline_info.pTessellationState = nullptr;
	pipeline_info.pViewportState = &viewport_info;
	pipeline_info.pRasterizationState = &rasterization_info;
	pipeline_info.pMultisampleState = &multisample_info;
	pipeline_info.pDepthStencilState = nullptr;
	pipeline_info.pColorBlendState = &color_blend_info;
	pipeline_info.pDynamicState = nullptr;
	pipeline_info.layout = m_pipeline_layout->get_pipeline_layout();
	pipeline_info.renderPass = m_render_pass->get_render_pass();
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;
	
	VK_DEBUG
		(	vkCreateGraphicsPipelines
		, "Failed to create graphics pipeline"
		, m_device.get_device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline)
	
}

vk::graphics_pipeline::~graphics_pipeline() {
	
	vkDestroyPipeline(m_device.get_device(), m_pipeline, nullptr);
	
	if (m_pipeline_layout) {
		
		delete m_pipeline_layout;
	}
	if (m_render_pass) {
		
		delete m_render_pass;
	}
	if (m_vertex_shader_module) {
		
		delete m_vertex_shader_module;
	}
	if (m_fragment_shader_module) {
		
		delete m_fragment_shader_module;
	}
}