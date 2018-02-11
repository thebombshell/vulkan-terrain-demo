
// vulkan_shader_module.hpp
//
// header file for the RAII wrapper of the VkShaderModule
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_SHADER_MODULE_HPP
#define VKCPP_VULKAN_SHADER_MODULE_HPP

#include "vulkan.hpp"

namespace vk {
	
	class shader_module {
		
		public:
		
		shader_module(vk::device& t_device, const char* t_path);
		~shader_module();
		
		vk::device& get_device();
		const vk::device& get_device() const;
		
		VkShaderModule get_shader_module() const;
		
		private:
		
		vk::device& m_device;
		VkShaderModule m_shader_module;
	};
}

#endif