
// vulkan_swapchain.cpp
//
// source file for the RAII wrapper of the VkSwapchainKHR
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_swapchain.hpp"
#include "vulkan_device.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_surface.hpp"

#include <algorithm>

vk::swapchain::swapchain(vk::surface& t_surface, vk::device& t_device) : device_object{t_device}, m_surface{t_surface} {
	
	choose_surface_format();
	choose_present_mode();
	choose_surface_extent();
	create_swapchain();
	create_swap_images();
}

vk::swapchain::~swapchain() {
	
	for (vk::image_view* t_image_view : m_image_views) {
		
		delete t_image_view;
	}
	m_image_views.clear();
	for (vk::image_reference* t_swap_image : m_swap_images) {
		
		delete t_swap_image;
	}
	m_swap_images.clear();
	vkDestroySwapchainKHR(m_device.get_device(), m_swapchain, nullptr);
}

void vk::swapchain::choose_surface_format() {
	
	m_surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
	m_surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	for (const auto& t_surface_format : m_surface.get_surface_formats()) {
		
		if (t_surface_format.format == m_surface_format.format && t_surface_format.colorSpace == m_surface_format.colorSpace) {
			
			return;
		}
	}
	
	m_surface_format = m_surface.get_surface_formats()[0];
}

VkExtent2D vk::swapchain::get_extent() const {
	
	return m_extent;
}

VkSurfaceFormatKHR vk::swapchain::get_surface_format() const {
	
	return m_surface_format;
}

VkPresentModeKHR vk::swapchain::get_present_mode() const {
	
	return m_present_mode;
}

VkSwapchainKHR vk::swapchain::get_swapchain() const {
	
	return m_swapchain;
}

void vk::swapchain::choose_present_mode() {
	
	m_present_mode = VK_PRESENT_MODE_FIFO_KHR;
	
	for (const auto& t_present_mode : m_surface.get_present_modes()) {
		
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
	
	VkSurfaceCapabilitiesKHR capabilities = m_surface.get_surface_capabilities();
	m_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, static_cast<uint32_t>(1280)));
	m_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(720)));
}

void vk::swapchain::create_swapchain() {
	
	VkSurfaceCapabilitiesKHR capabilities = m_surface.get_surface_capabilities();
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
	
	std::vector<uint32_t> family_indices = {static_cast<uint32_t>(m_device.get_graphical_queue_family_index()), static_cast<uint32_t>(m_device.get_present_queue_family_index())};
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
	
	std::vector<VkImage> swap_images{image_count};
	VK_DEBUG
		( vkGetSwapchainImagesKHR
		, "Failed to get swap chain images"
		, m_device.get_device(), m_swapchain, &image_count, swap_images.data())
	
	for (const VkImage& t_image : swap_images) {
		
		m_swap_images.push_back(new vk::image_reference(t_image));
		m_image_views.push_back(new vk::image_view(m_device, **(--m_swap_images.end()), m_surface_format.format));
	}
}

const std::vector<vk::image_reference*>& vk::swapchain::get_swap_images() {
	
	return m_swap_images;
}

const std::vector<vk::image_view*>& vk::swapchain::get_image_views() {
	
	return m_image_views;
}

uint32_t vk::swapchain::get_available_image_index(vk::semaphore* t_semaphore) {
	
	uint32_t output;
	VK_DEBUG
		( vkAcquireNextImageKHR
		, "Failed to acquire next image index"
		, m_device.get_device(), m_swapchain, std::numeric_limits<uint32_t>::max(), (t_semaphore ? t_semaphore->get_semaphore() : VK_NULL_HANDLE), VK_NULL_HANDLE, &output)
	return output;
}