
// vulkan_semaphore.hpp
//
// header file for the RAII wrapper of the VkSemaphore
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_SEMAPHORE_HPP
#define VKCPP_VULKAN_SEMAPHORE_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class semaphore : public i_device_object {
		
		public:
		
		semaphore(vk::device& t_device);
		~semaphore();
		
		VkSemaphore get_semaphore();
		
		private:
		
		VkSemaphore m_semaphore;
	};
}

#endif