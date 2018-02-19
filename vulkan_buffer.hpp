
// vulkan_buffer.hpp
//
// header file for the RAII wrapper of the VkBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_BUFFER_HPP
#define VKCPP_VULKAN_BUFFER_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class buffer : public device_object {
		
		public:
		
		buffer(vk::device& t_device, uint32_t t_size, VkBufferUsageFlags t_usage, VkSharingMode t_sharing_mode) ;
		~buffer();
		
		VkBuffer get_buffer();
		
		void map(const void* t_data_pointer, uint32_t t_data_size);
		
		protected:
		
		VkBuffer m_buffer;
		VkDeviceMemory m_device_memory;
	};
	
	class vertex_buffer : public buffer {
		
		public:
		
		vertex_buffer(vk::device& t_device, uint32_t t_size);
		~vertex_buffer();
		
		private:
		
	};
}

#endif