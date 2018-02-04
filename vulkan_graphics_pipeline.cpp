
// /vulkan_graphics_pipeline.cpp

#include "vulkan_graphics_pipeline.hpp"
#include "vulkan_device.hpp"

#include <fstream>

using namespace vk_terrain_demo;

vk::graphics_pipeline::graphics_pipeline(vk::device& t_device) : m_device{t_device} {
	
	create_shader_module_from_file("vert.spv", m_vertex_module);
	create_shader_module_from_file("frag.spv", m_fragment_module);
}

vk::graphics_pipeline::~graphics_pipeline() {
	
	vkDestroyShaderModule(m_device.get_device(), m_fragment_module, nullptr);
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