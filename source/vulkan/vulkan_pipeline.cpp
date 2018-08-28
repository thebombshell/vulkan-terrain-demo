
// vulkan_pipeline.cpp
//
// source file for the RAII wrrapper of the VkPipeline
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_pipeline.hpp"
#include "vulkan_device.hpp"
#include "vulkan_mesh.hpp"
#include "vulkan_pipeline_layout.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

vk::i_pipeline::i_pipeline() {
	
}

vk::i_pipeline::~i_pipeline() {
	
}

VkPipeline vk::i_pipeline::get_pipeline() {
	
	return m_pipeline;
}

const std::vector<VkDynamicState> g_dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

vk::graphics_pipeline::graphics_pipeline(vk::device& t_device, vk::render_pass& t_render_pass
	, VkExtent2D t_extent, VkFormat t_format
	, const std::vector<vk::shader_module*>& t_shader_modules
	, const vk::vertex_definition& t_definition)
	: i_device_object{t_device}, m_shader_modules{t_shader_modules.begin(), t_shader_modules.end()}
	, m_pipeline_layout{nullptr}, m_render_pass{t_render_pass} {
	
	VkPipelineVertexInputStateCreateInfo vertex_input_info;
	VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewport_info;
	VkPipelineRasterizationStateCreateInfo rasterization_info;
	VkPipelineMultisampleStateCreateInfo multisample_info;
	VkPipelineColorBlendAttachmentState color_blend_state;
	VkPipelineColorBlendStateCreateInfo color_blend_info;
	
	std::vector<VkPipelineShaderStageCreateInfo> shader_stage_info;
	shader_stage_info.resize(t_shader_modules.size());
	for (uint32_t i = 0; i < t_shader_modules.size(); ++i) {
		
		shader_stage_info[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_info[i].pNext = nullptr;
		shader_stage_info[i].flags = 0;
		shader_stage_info[i].stage = t_shader_modules[i]->get_shader_stage();
		shader_stage_info[i].module = t_shader_modules[i]->get_shader_module();
		shader_stage_info[i].pName = "main";
		shader_stage_info[i].pSpecializationInfo = nullptr;
	}
	
	std::vector<VkVertexInputBindingDescription> input_bindings;
	std::vector<VkVertexInputAttributeDescription> input_attributes;
	for (uint32_t i = 0; i < t_definition.bindings.size(); ++i) {
		
		const vk::vertex_binding& binding{t_definition.bindings[i]};
		VkVertexInputBindingDescription input_binding{};
		input_binding.binding = i;
		input_binding.stride = binding.stride;
		input_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		input_bindings.push_back(input_binding);
		for (uint32_t j = 0; j < binding.attributes.size(); ++j) {
			
			const vk::vertex_attribute& attribute{binding.attributes[j]};
			
			VkVertexInputAttributeDescription input_attribute{};
			input_attribute.location = i + j;
			input_attribute.binding = i;
			input_attribute.format = attribute.format;
			input_attribute.offset = attribute.offset;
			input_attributes.push_back(input_attribute);
		}
	}
	
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.pNext = nullptr;
	vertex_input_info.flags = 0;
	vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(input_bindings.size());
	vertex_input_info.pVertexBindingDescriptions = input_bindings.data();
	vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(input_attributes.size());
	vertex_input_info.pVertexAttributeDescriptions = input_attributes.data();
	
	input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_info.pNext = nullptr;
	input_assembly_info.flags = 0;
	input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly_info.primitiveRestartEnable = VK_FALSE;
	
	VkExtent2D extent = t_extent;
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
	rasterization_info.rasterizerDiscardEnable = VK_FALSE;
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
	
	m_pipeline_layout = new vk::pipeline_layout(m_device, nullptr, 0, nullptr, 0);
	
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
	pipeline_info.renderPass = m_render_pass.get_render_pass();
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
}

vk::render_pass& vk::graphics_pipeline::get_render_pass() {
	
	return m_render_pass;
}

const vk::render_pass& vk::graphics_pipeline::get_render_pass() const {
	
	return m_render_pass;
}