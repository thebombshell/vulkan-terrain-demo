
// vulkan_shader_module.cpp
//
// source file for the RAII wrapper of the VkShaderModule
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_shader_module.hpp"
#include "vulkan_device.hpp"

#include <fstream>

vk::shader_module::shader_module(vk::device& t_device, VkShaderStageFlagBits t_shader_stage, const char* t_path) 
	: device_object{t_device}, m_shader_stage{t_shader_stage} {
	
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
		, (std::string("Failed to create shader module at \"") + std::string(t_path) + "\"").c_str()
		, m_device.get_device(), &module_info, nullptr, &m_shader_module)
}

vk::shader_module::~shader_module() {
	
	vkDestroyShaderModule(m_device.get_device(), m_shader_module, nullptr);
}

VkShaderModule vk::shader_module::get_shader_module() const {
	
	return m_shader_module;
}