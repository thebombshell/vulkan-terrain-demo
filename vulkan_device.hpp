
// /vulkan_device.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_DEVICE_HPP
#define VK_TERRAIN_DEMO_VULKAN_DEVICE_HPP

#include "vulkan.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class device {
			
			public:
			
			device(vk::instance& t_instance, vk::surface& t_surface);
			~device();
			
			vk::instance& get_instance();
			vk::surface& get_surface();
			
			VkPhysicalDevice get_physical_device();
			const std::vector<VkQueueFamilyProperties>& get_queue_families() const;
			const std::vector<VkSurfaceFormatKHR>& get_surface_formats() const;
			const std::vector<VkPresentModeKHR>& get_present_modes() const;
			const VkSurfaceCapabilitiesKHR& get_capabilities() const;
			
			VkDevice get_device();
			VkQueue get_logical_queue();
			VkQueue get_present_queue();
			
			int get_graphics_family_index() const;
			int get_present_family_index() const;
			
			float get_queue_priority() const;
			void set_queue_priority(float t_value);
			
			private:
			
			int find_queue_families(VkPhysicalDevice& t_device);
			int find_surface_capabilities(VkPhysicalDevice& t_device);
			int is_physical_device_suitable(VkPhysicalDevice& t_device);
			int find_physical_device();
			void create_devices();
			
			vk::instance& m_instance;
			vk::surface& m_surface;
			
			VkPhysicalDevice m_physical_device;
			std::vector<VkQueueFamilyProperties> m_queue_families;
			std::vector<VkSurfaceFormatKHR> m_surface_formats;
			std::vector<VkPresentModeKHR> m_present_modes;
			VkSurfaceCapabilitiesKHR m_surface_capability;
			VkDevice m_device;
			VkQueue m_logical_queue;
			VkQueue m_present_queue;
			
			int m_graphics_family_index;
			int m_present_family_index;
			float m_queue_priority;
			
		};
	}
}

#endif