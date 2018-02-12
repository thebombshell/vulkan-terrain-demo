
// vulkan_context.cpp
//
// source file for the VKCPP all encompassing graphical context, intended to contain complex vulkan procedures and make
// graphics experiments easier and faster to produce
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_context.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_device.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_pipeline.hpp"

#include <algorithm>
#include <iostream>

vk::context::context(HWND t_window_handle, HINSTANCE t_handle_instance)
	: m_instance{nullptr}, m_surface{nullptr}, m_device{nullptr}, m_swapchain{nullptr}, m_pipeline{nullptr} {
	
	const char* const instance_layers[] = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	const char* const instance_extensions[] = {
		"VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};
	const char* const device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	m_instance = new vk::instance( instance_layers, 1, instance_extensions, 3);
	m_validation = new vk::validation(*m_instance);
	for (auto* t_physical_device : m_instance->get_physical_devices()) {
		
		if ( t_physical_device->is_discrete_gpu() 
			&& t_physical_device->has_geometry_shader()
			&& t_physical_device->are_extensions_supported(device_extensions, 1)) {
			
			m_surface = new vk::surface(t_window_handle, t_handle_instance, *m_instance, *t_physical_device);
			m_device = new vk::device(*t_physical_device, *m_surface, nullptr, 0, device_extensions, 1);
			break;
		}
	}
	m_swapchain = new vk::swapchain(*m_surface, *m_device);
	m_pipeline = new vk::graphics_pipeline(*m_device, *m_swapchain);
}

vk::context::~context() {
	
	if (m_pipeline) {
		
		delete m_pipeline;
	}
	
	if (m_swapchain) {
		
		delete m_swapchain;
	}
	
	if (m_device) {
		
		delete m_device;
	}
	
	if (m_surface) {
		
		delete m_surface;
	}
	
	if (m_instance) {
		
		delete m_instance;
	}
}
