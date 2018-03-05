
// vulkan_shader_module.hpp
//
// header file for the RAII wrapper of the VkShaderModule
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_SHADER_MODULE_HPP
#define VKCPP_VULKAN_SHADER_MODULE_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"
#include "vulkan_asset_manager.hpp"

namespace vk {
	
	class shader_module : public device_object {
		
		public:
		
		shader_module(vk::device& t_device, VkShaderStageFlagBits t_shader_stage, const char* t_path);
		~shader_module();
		
		VkShaderStageFlagBits get_shader_stage();
		VkShaderModule get_shader_module();
		
		private:
		
		VkShaderStageFlagBits m_shader_stage;
		VkShaderModule m_shader_module;
	};
}

#endif