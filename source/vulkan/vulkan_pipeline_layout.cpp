
// vulkan_pipeline_layout.cpp
//
// source file for the RAII wrrapper of the VkPipelineLayout
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_descriptor_set_layout.hpp"
#include "vulkan_pipeline_layout.hpp"

//
// pipeline_layout
//

vk::pipeline_layout::pipeline_layout
	( vk::device& t_device
	, std::vector<vk::descriptor_set_layout*>& t_descriptors
	, const VkPushConstantRange* t_push_constant_ranges, uint32_t t_push_constant_range_count
	) : i_device_object{t_device} {
	
	std::vector<VkDescriptorSetLayout> descriptors{t_descriptors.size()};
	for (uint32_t i = 0; i < descriptors.size(); ++i) {
		
		descriptors[i] = t_descriptors[i]->get_descriptor_set_layout();
	}
	
	VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.pNext = nullptr;
	pipeline_layout_info.flags = 0;
	pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(descriptors.size());
	pipeline_layout_info.pSetLayouts = &descriptors[0];
	pipeline_layout_info.pushConstantRangeCount = t_push_constant_range_count;
	pipeline_layout_info.pPushConstantRanges = t_push_constant_ranges;
	
	VK_DEBUG
		( vkCreatePipelineLayout
		, "Failed to create pipeline layout"
		, m_device.get_device(), &pipeline_layout_info, nullptr, &m_pipeline_layout)
}

vk::pipeline_layout::~pipeline_layout() {
	
	vkDestroyPipelineLayout(m_device.get_device(), m_pipeline_layout, nullptr);
}

VkPipelineLayout vk::pipeline_layout::get_pipeline_layout() const {
	
	return m_pipeline_layout;
}