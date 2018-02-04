
// /vulkan_context.cpp

#include "vulkan_context.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_device.hpp"
#include "vulkan_swapchain.hpp"

#include <algorithm>
#include <iostream>

using namespace vk_terrain_demo;

vk::context::context(window& t_window) 
	: m_window{t_window}, m_instance{nullptr}, m_surface{nullptr}, m_device{nullptr}, m_swapchain{nullptr} {
	
	if (!is_validation_supported()) {
		
		throw std::runtime_error("Requested validation layers are not supported.");
	}
	m_instance = new vk::instance();
	m_surface = new vk::surface(m_window, *m_instance);
	m_device = new vk::device(*m_instance, *m_surface);
	m_swapchain = new vk::swapchain(*m_surface, *m_device);
}

vk::context::~context() {
	
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

int vk::context::is_validation_supported() {
	
	uint32_t layer_count;
	VK_DEBUG
		( vkEnumerateInstanceLayerProperties
		, "Failed to get instance layer count"
		, &layer_count, nullptr)
	
	std::vector<VkLayerProperties> layer_properties{layer_count};
	VK_DEBUG
		( vkEnumerateInstanceLayerProperties
		, "Failed to get instance layer properties"
		, &layer_count, layer_properties.data())
	
	uint32_t matching_layer_count = 0;
	auto begin = g_validation_layer_strings.begin();
	auto end = g_validation_layer_strings.end();
	
	for (const auto& t_property : layer_properties) {
		
		if (std::find(begin, end, std::string(t_property.layerName)) != end) {
			
			++matching_layer_count;
		}
	}
	
	return matching_layer_count == g_validation_layer_strings.size();
}