
// vulkan_device.hpp
//
// header file for the RAII wrapper of the VkPhysicalDevice and VkDevice
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_DEVICE_HPP
#define VKCPP_VULKAN_DEVICE_HPP

#include "vulkan.hpp"

namespace vk {
	
	class device {
		
		public:
		
		device
			( vk::physical_device& t_physical_device
			, vk::surface& t_surface
			, const char* const* t_layer_names, uint32_t t_layer_count
			, const char* const* t_extension_names, uint32_t t_extension_count);
		~device();
		
		vk::physical_device& get_physical_device();
		const vk::physical_device& get_physical_device() const;
		VkDevice get_device() const;
		VkQueue get_graphical_queue() const;
		VkQueue get_present_queue() const;
		
		int get_graphical_queue_family_index() const;
		int get_present_queue_family_index() const;
		
		float get_queue_priority() const;
		void set_queue_priority(float t_value);
		
		void submit_graphical_queue( const VkSubmitInfo* t_submit_infos, uint32_t t_submit_info_count = 1, vk::fence* t_fence = nullptr);
		void queue_pressent(std::vector<VkSemaphore>& t_semaphores, std::vector<VkSwapchainKHR>& t_swapchains, std::vector<uint32_t>& t_image_indices);
		
		private:
		
		vk::physical_device& m_physical_device;
		vk::surface& m_surface;
		
		VkDevice m_device;
		int m_graphical_queue_family_index;
		int m_present_queue_family_index;
		VkQueue m_graphical_queue;
		VkQueue m_present_queue;
	};
	
	class i_device_object {
		
		public:
		
		virtual ~i_device_object();
		
		vk::device& get_device();
		const vk::device& get_device() const;
		
		protected:
		
		i_device_object(vk::device& t_device);
		i_device_object(vk::i_device_object& t_i_device_object);
		vk::device& m_device;
	};
}

#endif