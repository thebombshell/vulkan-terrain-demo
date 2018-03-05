
// vulkan_fence.hpp
//
// header file for the RAII wrapper of the VkFence
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_FENCE_HPP
#define VKCPP_VULKAN_FENCE_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class fence : public i_device_object {
		
		public:
		
		fence(vk::device& t_device);
		~fence();
		
		VkFence get_fence();
		
		private:
		
		VkFence m_fence;
	};
}

#endif