
// /vulkan_graphics_pipeline.cpp

#include "vulkan_graphics_pipeline.hpp"
#include "vulkan_device.hpp"
#include "vulkan_swapchain.hpp"

#include <fstream>

const std::vector<VkDynamicState> g_dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

using namespace vk_terrain_demo;

vk::graphics_pipeline::graphics_pipeline(vk::device& t_device, vk::swapchain& t_swapchain) : m_device{t_device}, m_swapchain{t_swapchain} {
	
	create_shader_module_from_file("vert.spv", m_vertex_module);
	create_shader_module_from_file("frag.spv", m_fragment_module);
	setup_pipeline_info();
	create_pipeline_layout();
}

vk::graphics_pipeline::~graphics_pipeline() {
	
	vkDestroyPipelineLayout(m_device.get_device(), m_pipeline_layout, nullptr);
	vkDestroyShaderModule(m_device.get_device(), m_vertex_module, nullptr);
	vkDestroyShaderModule(m_device.get_device(), m_fragment_module, nullptr);
}

void vk::graphics_pipeline::create_shader_module_from_file(const char* t_path, VkShaderModule& t_shader_module) {
	
	std::ifstream file(t_path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		
		throw std::runtime_error(std::string("Could not load shader \"") + t_path + "\"");
	}
	
	VkShaderModuleCreateInfo module_info = {};
	module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	module_info.pNext = nullptr;
	module_info.flags = 0;
	module_info.codeSize = size;
	module_info.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
	VK_DEBUG
		(vkCreateShaderModule
		, std::string("Failed to create shader module at \"") + std::string(t_path) + "\""
		, m_device.get_device(), &module_info, nullptr, &t_shader_module)
}

void vk::graphics_pipeline::setup_pipeline_info() {
	
	m_vertex_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_vertex_stage_info.pNext = nullptr;
	m_vertex_stage_info.flags = 0;
	m_vertex_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	m_vertex_stage_info.module = m_vertex_module;
	m_vertex_stage_info.pName = "main";
	m_vertex_stage_info.pSpecializationInfo = nullptr;
	
	m_fragment_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_fragment_stage_info.pNext = nullptr;
	m_fragment_stage_info.flags = 0;
	m_fragment_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	m_fragment_stage_info.module = m_fragment_module;
	m_fragment_stage_info.pName = "main";
	m_fragment_stage_info.pSpecializationInfo = nullptr;
	
	m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertex_input_info.pNext = nullptr;
	m_vertex_input_info.flags = 0;
	m_vertex_input_info.vertexBindingDescriptionCount = 0;
	m_vertex_input_info.pVertexBindingDescriptions = nullptr;
	m_vertex_input_info.vertexAttributeDescriptionCount = 0;
	m_vertex_input_info.pVertexAttributeDescriptions = nullptr;
	
	m_input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_input_assembly_info.pNext = nullptr;
	m_input_assembly_info.flags = 0;
	m_input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	m_input_assembly_info.primitiveRestartEnable = VK_TRUE;
	
	VkExtent2D extent = m_swapchain.get_extent();
	m_viewport.x = 0;
	m_viewport.y = 0;
	m_viewport.width = static_cast<float>(extent.width);
	m_viewport.height = static_cast<float>(extent.height);
	m_viewport.minDepth = 0.0;
	m_viewport.maxDepth = 1.0;
	
	m_scissor.offset = {0, 0};
	m_scissor.extent = extent;
	
	m_viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewport_info.pNext = nullptr;
	m_viewport_info.flags = 0;
	m_viewport_info.viewportCount = 1;
	m_viewport_info.pViewports = &m_viewport;
	m_viewport_info.scissorCount = 1;
	m_viewport_info.pScissors = &m_scissor;
	
	m_rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_rasterization_info.pNext = nullptr;
	m_rasterization_info.flags = 0;
	m_rasterization_info.depthClampEnable = VK_FALSE;
	m_rasterization_info.rasterizerDiscardEnable = VK_TRUE;
	m_rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
	m_rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
	m_rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	m_rasterization_info.depthBiasEnable = VK_FALSE;
	m_rasterization_info.depthBiasConstantFactor = 0.0f;
	m_rasterization_info.depthBiasClamp = 0.0f;
	m_rasterization_info.depthBiasSlopeFactor = 0.0f;
	m_rasterization_info.lineWidth = 1.0f;
	
	m_multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_multisample_info.pNext = nullptr;
	m_multisample_info.flags = 0;
	m_multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	m_multisample_info.sampleShadingEnable = VK_FALSE;
	m_multisample_info.minSampleShading = 1.0f;
	m_multisample_info.pSampleMask = nullptr;
	m_multisample_info.alphaToCoverageEnable = VK_FALSE;
	m_multisample_info.alphaToOneEnable = VK_FALSE;
	
	m_color_blend_state.blendEnable = VK_FALSE;
	m_color_blend_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	m_color_blend_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	m_color_blend_state.colorBlendOp = VK_BLEND_OP_ADD;
	m_color_blend_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	m_color_blend_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	m_color_blend_state.alphaBlendOp = VK_BLEND_OP_ADD;
	m_color_blend_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	
	m_color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_color_blend_info.pNext = nullptr;
	m_color_blend_info.flags = 0;
	m_color_blend_info.logicOpEnable = VK_FALSE;
	m_color_blend_info.logicOp = VK_LOGIC_OP_COPY;
	m_color_blend_info.attachmentCount = 1;
	m_color_blend_info.pAttachments = &m_color_blend_state;
	m_color_blend_info.blendConstants[0] = 0.0f;
	m_color_blend_info.blendConstants[1] = 0.0f;
	m_color_blend_info.blendConstants[2] = 0.0f;
	m_color_blend_info.blendConstants[3] = 0.0f;
	
	m_dymanic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	m_dymanic_state_info.pNext = nullptr;
	m_dymanic_state_info.flags = 0;
	m_dymanic_state_info.dynamicStateCount = static_cast<uint32_t>(g_dynamic_states.size());
	m_dymanic_state_info.pDynamicStates = g_dynamic_states.data();
}

void vk::graphics_pipeline::create_pipeline_layout() {
	
	VkPipelineLayoutCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_info.pNext = nullptr;
	pipeline_info.flags = 0;
	pipeline_info.setLayoutCount = 0;
	pipeline_info.pSetLayouts = nullptr;
	pipeline_info.pushConstantRangeCount = 0;
	pipeline_info.pPushConstantRanges = 0;

	VK_DEBUG
		( vkCreatePipelineLayout
		, "Failed to create pipeline layout"
		, m_device.get_device(), &pipeline_info, nullptr, &m_pipeline_layout)
}