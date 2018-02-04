
// /vulkan_device.cpp

#include "vulkan_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"

#include <algorithm>
#include <assert.h>

using namespace vk_terrain_demo;

vk::device::device(vk::instance& t_instance, vk::surface& t_surface)
	: m_instance{t_instance}, m_surface{t_surface}, m_graphics_family_index{-1}, m_present_family_index{-1}, m_queue_priority{1.0f} {
	
	if (!find_physical_device()) {
		
		throw std::runtime_error("Failed to find suitable physical device.");
	}
	
	create_devices();
}

vk::device::~device() {
	
	vkDestroyDevice(m_device, nullptr);
}

const std::vector<VkQueueFamilyProperties>& vk::device::get_queue_families() const {
	
	return m_queue_families;
}

const std::vector<VkSurfaceFormatKHR>& vk::device::get_surface_formats() const {
	
	return m_surface_formats;
}

const std::vector<VkPresentModeKHR>& vk::device::get_present_modes() const {
	
	return m_present_modes;
}
const VkSurfaceCapabilitiesKHR& vk::device::get_capabilities() const {
	
	return m_surface_capability;
}

VkDevice vk::device::get_device() {
	
	return m_device;
}

VkQueue vk::device::get_logical_queue() {
	
	return m_logical_queue;
}

VkQueue vk::device::get_present_queue() {
	
	return m_present_queue;
}

VkPhysicalDevice vk::device::get_physical_device() {
	
	return m_physical_device;
}

int vk::device::get_graphics_family_index() const {
	
	return m_graphics_family_index;
}
int vk::device::get_present_family_index() const {
	
	return m_present_family_index;
}

float vk::device::get_queue_priority() const {
	
	return m_queue_priority;
}

void vk::device::set_queue_priority(float t_value) {
	
	assert(t_value > 0.0f && t_value < 1.0f && "Queue priority must remain between 0.0 and 1.0.");
	m_queue_priority = t_value;
}

int vk::device::find_queue_families(VkPhysicalDevice& t_device) {
	
	uint32_t family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(t_device, &family_count, nullptr);
	
	m_queue_families.resize(family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(t_device, &family_count, m_queue_families.data());
	
	int i = 0;
	for (const auto& t_queue_family : m_queue_families) {
		
		if (t_queue_family.queueCount > 0 && t_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			
			m_graphics_family_index = i;
		}
		
		VkBool32 present_support = false;
		VK_DEBUG
			( vkGetPhysicalDeviceSurfaceSupportKHR
			, "Failed to get physical device surface support"
			, t_device, i, m_surface.get_surface(), &present_support)
		if (t_queue_family.queueCount > 0 && present_support) {
			
			m_present_family_index = i;
		}
		
		++i;
	}
	
	return m_graphics_family_index >= 0 && m_present_family_index >= 0;
}

int vk::device::find_surface_capabilities(VkPhysicalDevice& t_device) {
	
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceCapabilitiesKHR
		, "Failed to get physical device surface capabilities"
		, t_device, m_surface.get_surface(), &m_surface_capability )
	
	uint32_t format_count;
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceFormatsKHR
		, "Failed to get physical device surface format count"
		, t_device, m_surface.get_surface(), &format_count, nullptr);
	
	m_surface_formats.resize(format_count);
	VK_DEBUG
		( vkGetPhysicalDeviceSurfaceFormatsKHR
		, "Failed to get physical device surface formats"
		, t_device, m_surface.get_surface(), &format_count, m_surface_formats.data());
	
	uint32_t mode_count;
	VK_DEBUG
		( vkGetPhysicalDeviceSurfacePresentModesKHR
		, "Failed to get physical device surface present mode count"
		, t_device, m_surface.get_surface(), &mode_count, nullptr);
	
	m_present_modes.resize(mode_count);
	VK_DEBUG
		( vkGetPhysicalDeviceSurfacePresentModesKHR
		, "Failed to get physical device surface present modes"
		, t_device, m_surface.get_surface(), &mode_count, m_present_modes.data());
	
	return format_count && mode_count;
}

int vk::device::is_physical_device_suitable(VkPhysicalDevice& t_device) {
	
	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceProperties(t_device, &device_properties);
	vkGetPhysicalDeviceFeatures(t_device, &device_features);
	
	uint32_t extension_count;
	VK_DEBUG
		( vkEnumerateDeviceExtensionProperties
		, "Failed to get device extension count"
		, t_device, nullptr, &extension_count, nullptr)
	
	std::vector<VkExtensionProperties> device_extensions(extension_count);
	VK_DEBUG
		( vkEnumerateDeviceExtensionProperties
		, "Failed to get device extension properties"
		, t_device, nullptr, &extension_count, device_extensions.data())
	
	extension_count = 0;
	auto begin = g_device_extension_strings.begin();
	auto end = g_device_extension_strings.end();
	
	for (const VkExtensionProperties& t_property : device_extensions) {
		
		if (std::find(begin, end, std::string(t_property.extensionName)) != end) {
			
			++extension_count;
		}
	}
	
	return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
		&& device_features.geometryShader
		&& extension_count == g_device_extension_strings.size()
		&& find_surface_capabilities(t_device)
		&& find_queue_families(t_device);
}

int vk::device::find_physical_device() {
	
	uint32_t device_count = 0;
	VK_DEBUG
		( vkEnumeratePhysicalDevices
		, "Failed to get physical device count"
		, m_instance.get_instance(), &device_count, nullptr)
	
	if (!device_count) {
		
		return 0;
	}
	
	std::vector<VkPhysicalDevice> devices(device_count);
	VK_DEBUG
		( vkEnumeratePhysicalDevices
		, "Failed to get physical devices"
		, m_instance.get_instance(), &device_count, devices.data())
	
	for (VkPhysicalDevice& physical_device : devices) {
		
		if (is_physical_device_suitable(physical_device)) {
			
			m_physical_device = physical_device;
			return 1;
		}
	}
	
	return 0;
}

void vk::device::create_devices() {
	
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	
	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.pNext = NULL;
	queue_create_info.flags = 0;
	queue_create_info.queueFamilyIndex = m_graphics_family_index;
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = &m_queue_priority;
	queue_create_infos.push_back(queue_create_info);
	if (m_graphics_family_index != m_present_family_index) {
		
		queue_create_info = {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.pNext = NULL;
		queue_create_info.flags = 0;
		queue_create_info.queueFamilyIndex = m_present_family_index;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &m_queue_priority;
		queue_create_infos.push_back(queue_create_info);
	}
	VkPhysicalDeviceFeatures device_features = {};
	
	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext = nullptr;
	device_create_info.flags = 0;
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	device_create_info.pQueueCreateInfos = queue_create_infos.data();
	device_create_info.enabledLayerCount = static_cast<uint32_t>(g_validation_layer_chars.size());
	device_create_info.ppEnabledLayerNames = g_validation_layer_chars.data();
	device_create_info.enabledExtensionCount = static_cast<uint32_t>(g_device_extension_chars.size());
	device_create_info.ppEnabledExtensionNames = g_device_extension_chars.data();
	device_create_info.pEnabledFeatures = &device_features;
	
	VK_DEBUG
		( vkCreateDevice
		, "Failed to create logical device"
		, m_physical_device, &device_create_info, nullptr, &m_device)
	vkGetDeviceQueue(m_device, m_graphics_family_index, 0, &m_logical_queue);
	vkGetDeviceQueue(m_device, m_present_family_index, 0, &m_present_queue);
}