
// /vulkan_swapchain.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_SWAPCHAIN_HPP
#define VK_TERRAIN_DEMO_VULKAN_SWAPCHAIN_HPP

#include "vulkan.hpp"
#include "vulkan_device.hpp"

namespace vk {
	
	class swapchain : public device_object {
		
		public:
		
		swapchain(vk::surface& t_surface, vk::device& t_device);
		~swapchain();
		
		vk::surface& get_surface();
		
		VkExtent2D get_extent() const;
		VkSurfaceFormatKHR get_surface_format() const;
		VkPresentModeKHR get_present_mode() const;
		VkSwapchainKHR get_swapchain() const;
		
		private:
		
		void choose_surface_extent();
		void choose_surface_format();
		void choose_present_mode();
		void create_swapchain();
		void create_swap_images();
		
		vk::surface& m_surface;
		
		VkExtent2D m_extent;
		VkSurfaceFormatKHR m_surface_format;
		VkPresentModeKHR m_present_mode;
		VkSwapchainKHR m_swapchain;
		
		std::vector<vk::image_reference*> m_swap_images;
		std::vector<vk::image_view*> m_image_views;
	};
}

#endif