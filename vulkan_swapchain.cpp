
// /vulkan_swapchain.hpp

#include "vulkan_swapchain.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_device.hpp"

using namespace vk_terrain_demo;

vk::swapchain::swapchain(vk::surface& t_surface, vk::device& t_device) : m_surface{t_surface}, m_device{t_device} {
	
	choose_surface_format();
	choose_present_mode();
	choose_surface_extent();
	create_swapchain();
	create_swap_images();
}

vk::swapchain::~swapchain() {
	
	for (VkImageView& t_image_view : m_image_views) {
		
		vkDestroyImageView(m_device.get_device(), t_image_view, nullptr);
	}
	vkDestroySwapchainKHR(m_device.get_device(), m_swapchain, nullptr);
}

void vk::swapchain::choose_surface_format() {
	
	m_surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
	m_surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	for (const auto& t_surface_format : m_device.get_surface_formats()) {
		
		if (t_surface_format.format == m_surface_format.format && t_surface_format.colorSpace == m_surface_format.colorSpace) {
			
			return;
		}
	}
	
	m_surface_format = m_device.get_surface_formats()[0];
}

void vk::swapchain::choose_present_mode() {
	
	m_present_mode = VK_PRESENT_MODE_FIFO_KHR;
	
	for (const auto& t_present_mode : m_device.get_present_modes()) {
		
		if (t_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			
			m_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		else if (m_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR
			|| t_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			
			m_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
		else if (m_present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR
			|| t_present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
			
			m_present_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		}
	}
}

void vk::swapchain::choose_surface_extent() {
	
	m_extent = m_device.get_capabilities().currentExtent;
}

void vk::swapchain::create_swapchain() {
	
	const VkSurfaceCapabilitiesKHR& capabilities = m_device.get_capabilities();
	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext = nullptr;
	swapchain_info.flags = 0;
	swapchain_info.surface = m_surface.get_surface();
	swapchain_info.minImageCount = capabilities.maxImageCount;
	swapchain_info.imageFormat = m_surface_format.format;
	swapchain_info.imageColorSpace = m_surface_format.colorSpace;
	swapchain_info.imageExtent = m_extent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	std::vector<uint32_t> family_indices = {static_cast<uint32_t>(m_device.get_graphics_family_index()), static_cast<uint32_t>(m_device.get_present_family_index())};
	if (family_indices[0] != family_indices[1]) {
		
		swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount = 2;
		swapchain_info.pQueueFamilyIndices = family_indices.data();
	}
	else {
		
		swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount = 0;
		swapchain_info.pQueueFamilyIndices = nullptr;
	}
	swapchain_info.preTransform = capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = m_present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;
	
	VK_DEBUG
		( vkCreateSwapchainKHR
		, "Failed to create swap chain"
		, m_device.get_device(), &swapchain_info, nullptr, &m_swapchain)
}

void vk::swapchain::create_swap_images() {
	
	uint32_t image_count;
	VK_DEBUG
		( vkGetSwapchainImagesKHR
		, "Failed to get swap chain image count"
		, m_device.get_device(), m_swapchain, &image_count, nullptr)
	
	m_swap_images.resize(image_count);
	VK_DEBUG
		( vkGetSwapchainImagesKHR
		, "Failed to get swap chain images"
		, m_device.get_device(), m_swapchain, &image_count, m_swap_images.data())
	
	m_image_views.resize(image_count);
	auto iter = m_image_views.begin();
	for (VkImage& t_image : m_swap_images) {
		
		VkImageViewCreateInfo image_view_info = {};
		image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_info.pNext = nullptr;
		image_view_info.flags = 0;
		image_view_info.image = t_image;
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = m_surface_format.format;
		image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.baseMipLevel = 0;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.baseArrayLayer = 0;
		image_view_info.subresourceRange.layerCount = 1;
		
		VK_DEBUG
			( vkCreateImageView
			, "Failed to create swap chain image view"
			, m_device.get_device(), &image_view_info, nullptr, &(*iter))
		++iter;
	}
}
