
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
	
	class i_buffer : public i_device_object {
		
		public:
		
		i_buffer(vk::device& t_device);
		virtual ~i_buffer() = 0;
		
		virtual VkBuffer get_buffer() = 0;
	};
	
	class buffer : public i_buffer {
		
		public:
		
		buffer(vk::device& t_device, uint32_t t_size, VkBufferUsageFlags t_usage, VkSharingMode t_sharing_mode,  uint32_t t_queue_family_count = 0, const uint32_t* t_queue_families = nullptr);
		virtual ~buffer();
		
		virtual VkBuffer get_buffer();
		virtual VkDeviceMemory get_device_memory();
		
		void map(const void* t_data_pointer, uint32_t t_data_size);
		
		protected:
		
		VkBuffer m_buffer;
		VkDeviceMemory m_device_memory;
	};
	
	class staging_buffer : public buffer {
		
		public:
		
		staging_buffer(vk::device& t_device, uint32_t t_size);
		virtual ~staging_buffer();
	};
	
	class staged_buffer : public i_buffer {
		
		public:
		
		staged_buffer(vk::device& t_device, vk::i_buffer& t_source, uint32_t t_size, VkBufferUsageFlags t_usage);
		virtual ~staged_buffer();
		
		VkBuffer get_buffer() override;
		vk::i_buffer& get_staging_buffer();
		const vk::i_buffer& get_staging_buffer() const;
		
		private:
		
		vk::i_buffer& m_source;
		VkBuffer m_destination_buffer;
		VkDeviceMemory m_destination_device_memory;
	};
	
	class vertex_buffer : public buffer {
		
		public:
		
		vertex_buffer(vk::device& t_device, uint32_t t_size);
		virtual ~vertex_buffer();
		
		private:
		
	};
	
	class index_buffer : public buffer {
		
		public:
		
		index_buffer(vk::device& t_device, uint32_t t_size);
		virtual ~index_buffer();
		
		private:
		
	};
	
	class staged_vertex_buffer : public staged_buffer {
		
		public:
		
		staged_vertex_buffer(vk::device& t_device, vk::i_buffer& t_source, uint32_t t_size);
		virtual ~staged_vertex_buffer();
		
		private:
		
	};
	
	class staged_index_buffer : public staged_buffer {
		
		public:
		
		staged_index_buffer(vk::device& t_device, vk::i_buffer& t_source, uint32_t t_size);
		virtual ~staged_index_buffer();
		
		private:
		
	};
	
}

#endif