
// /vulkan_graphics_pipeline.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP
#define VK_TERRAIN_DEMO_VULKAN_GRAPHICS_PIPELINE_HPP

#include "vulkan.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class graphics_pipeline {
			
			public:
			
			graphics_pipeline(vk::device& t_device, vk::swapchain& t_swapchain);
			~graphics_pipeline();
			
			private:
			
			void create_shader_module_from_file(const char* t_path, VkShaderModule& t_shader_module);
			void setup_pipeline_info();
			void create_pipeline_layout();
			
			vk::device& m_device;
			vk::swapchain& m_swapchain;
			
			VkShaderModule m_vertex_module;
			VkShaderModule m_fragment_module;
			VkPipelineShaderStageCreateInfo m_vertex_stage_info;
			VkPipelineShaderStageCreateInfo m_fragment_stage_info;
			VkPipelineVertexInputStateCreateInfo m_vertex_input_info;
			VkPipelineInputAssemblyStateCreateInfo m_input_assembly_info;
			VkViewport m_viewport;
			VkRect2D m_scissor;
			VkPipelineViewportStateCreateInfo m_viewport_info;
			VkPipelineRasterizationStateCreateInfo m_rasterization_info;
			VkPipelineMultisampleStateCreateInfo m_multisample_info;
			VkPipelineColorBlendAttachmentState m_color_blend_state;
			VkPipelineColorBlendStateCreateInfo m_color_blend_info;
			VkPipelineDynamicStateCreateInfo m_dymanic_state_info;
			VkPipelineLayout m_pipeline_layout;
		};
	}
}

#endif