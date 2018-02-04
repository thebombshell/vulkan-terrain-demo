
// /vulkan_swapchain.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_SWAPCHAIN_HPP
#define VK_TERRAIN_DEMO_VULKAN_SWAPCHAIN_HPP

#include "vulkan.hpp"

namespace vk_terrain_demo {
	
	namespace vk {
		
		class swapchain {
			
			public:
			
			swapchain(vk::surface& t_surface, vk::device& t_device);
			~swapchain();
			
			vk::surface& get_surface();
			vk::device& get_device();
			
			private:
			
			void choose_surface_extent();
			void choose_surface_format();
			void choose_present_mode();
			void create_swapchain();
			void create_swap_images();
			
			vk::surface& m_surface;
			vk::device& m_device;
			
			VkExtent2D m_extent;
			VkSurfaceFormatKHR m_surface_format;
			VkPresentModeKHR m_present_mode;
			VkSwapchainKHR m_swapchain;
			
			std::vector<VkImage> m_swap_images;
			std::vector<VkImageView> m_image_views;
		};
	}
}

#endif