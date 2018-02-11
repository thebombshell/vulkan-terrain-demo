
// vulkan_surface.cpp
//
// source file for the RAII wrapper of the vulkan surface extended classes
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_surface.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"

vk::surface::surface
	( HWND t_window_handle, HINSTANCE t_instance_handle
	, vk::instance& t_instance, vk::physical_device& t_physical_device) 
	: m_window_handle{t_window_handle}, m_instance_handle{t_instance_handle}
	, m_instance{t_instance}, m_physical_device{t_physical_device} {
	
	VkWin32SurfaceCreateInfoKHR surface_info = {};
	surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_info.pNext = nullptr;
	surface_info.flags = 0;
	surface_info.hinstance = m_instance_handle;
	surface_info.hwnd = m_window_handle;
	auto create_win32_surface = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(m_instance.get_instance(), "vkCreateWin32SurfaceKHR");
	
	if (create_win32_surface) {
		
		VK_DEBUG
			( create_win32_surface
			, "Failed to create win32 surface"
			, m_instance.get_instance(), &surface_info, nullptr, &m_surface)
	}
	else {
		
		throw vk::vulkan_exception("Failed to get instance procedure address", VK_SUCCESS);
	}
	
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceCapabilitiesKHR
		, "Failed to get physical device surface capabilities"
		, m_physical_device.get_physical_device(), m_surface, &m_surface_capabilities)
	
	uint32_t surface_format_count;
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceFormatsKHR
		, "Failed to get physical device surface format count"
		, m_physical_device.get_physical_device(), m_surface, &surface_format_count, nullptr)
	m_surface_formats.resize(surface_format_count);
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceFormatsKHR
		, "Failed to get physical device surface formats"
		, m_physical_device.get_physical_device(), m_surface, &surface_format_count, m_surface_formats.data())
	
	uint32_t present_mode_count;
	VK_DEBUG
		( vkGetPhysicalDeviceSurfacePresentModesKHR
		, "Failed to get physical device surface present mode count"
		, m_physical_device.get_physical_device(), m_surface, &present_mode_count, nullptr)
	m_present_modes.resize(present_mode_count);
	VK_DEBUG
		( vkGetPhysicalDeviceSurfacePresentModesKHR
		, "Failed to get physical device surface present modes"
		, m_physical_device.get_physical_device(), m_surface, &present_mode_count, m_present_modes.data())
	
}

vk::surface::~surface() {
	
	vkDestroySurfaceKHR(m_instance.get_instance(), m_surface, nullptr);
}

vk::instance& vk::surface::get_instance() {
	
	return m_instance;
}

const vk::instance& vk::surface::get_instance() const {
	
	return m_instance;
}

vk::physical_device& vk::surface::get_physical_device() {
	
	return m_physical_device;
}

const vk::physical_device& vk::surface::get_physical_device() const {
	
	return m_physical_device;
}

VkSurfaceKHR vk::surface::get_surface() const {
	
	return m_surface;
}

VkSurfaceCapabilitiesKHR vk::surface::get_surface_capabilities() const {
	
	return m_surface_capabilities;
}

const std::vector<VkSurfaceFormatKHR>& vk::surface::get_surface_formats() const {
	
	return m_surface_formats;
}

const std::vector<VkPresentModeKHR>& vk::surface::get_present_modes() const {
	
	return m_present_modes;
}

VkBool32 vk::surface::is_surface_supported_by_queue_family(uint32_t t_queue_family_index) const {
	
	VkBool32 is_true;
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceSupportKHR
		, "Failed to get physical device support"
		, m_physical_device.get_physical_device(), t_queue_family_index, m_surface, &is_true )
	return is_true;
}