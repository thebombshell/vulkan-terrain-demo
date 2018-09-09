
// vulkan_descriptor_pool.hpp
//
// header file for the RAII wrapper of the VkDescriptorPool
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_DESCRIPTOR_POOL_HPP
#define VKCPP_VULKAN_DESCRIPTOR_POOL_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class descriptor_pool : public i_device_object {
		
		public:
		
		descriptor_pool(vk::device& t_device, uint32_t t_max_sets, std::vector<VkDescriptorPoolSize>& t_pool_sizes);
		~descriptor_pool();
		
		VkDescriptorPool get_descriptor_pool();
		
		private:
		
		VkDescriptorPool m_descriptor_pool;
	};
}

#endif