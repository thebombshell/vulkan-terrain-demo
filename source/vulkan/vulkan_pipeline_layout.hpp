
// vulkan_pipeline_layout.hpp
//
// header file for the RAII wrrapper of the VkPipelineLayout
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_PIPELINE_LAYOUT_HPP
#define VKCPP_VULKAN_PIPELINE_LAYOUT_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class pipeline_layout : public i_device_object {
		
		public:
		
		pipeline_layout
			( vk::device& t_device
			, const VkDescriptorSetLayout* t_set_layouts = nullptr, uint32_t t_set_layout_count = 0
			, const VkPushConstantRange* t_push_constant_ranges = nullptr, uint32_t t_push_constant_range_count = 0);
		~pipeline_layout();
		
		VkPipelineLayout get_pipeline_layout();
		
		private:
		
		VkPipelineLayout m_pipeline_layout;
	};
}

#endif