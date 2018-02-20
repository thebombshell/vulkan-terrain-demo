
// vulkan_command_pool.hpp
//
// header file for the RAII wrrapper of the VkCommandPool
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_COMMAND_POOL_HPP
#define VKCPP_VULKAN_COMMAND_POOL_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class command_pool : public device_object {
		
		public:
		
		command_pool(vk::device& t_device, uint32_t t_queue_family_index);
		~command_pool();
		
		VkCommandPool get_command_pool();
		
		private:
		
		VkCommandPool m_command_pool;
	};
}

#endif