
// vulkan_instance.cpp
//
// source file for the RAII wrapper of the VkInstance
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

VKCPP_FLAG vk::are_instance_layers_supported(const char* const* t_layer_names, uint32_t t_layer_count) {
	
	// get layer property count
	
	uint32_t instance_layers_count;
	VK_DEBUG
		( vkEnumerateInstanceLayerProperties
		, "Failed to get instance layer count"
		, &instance_layers_count, nullptr);
	
	// copy layer properties into usable memory
	
	std::vector<VkLayerProperties> layer_properties{instance_layers_count};
	VK_DEBUG
		( vkEnumerateInstanceLayerProperties
		, "Failed to get instance layer properties"
		, &instance_layers_count, layer_properties.data());
	
	// store iterators
	
	auto layer_properties_begin = layer_properties.begin();
	auto layer_properties_end = layer_properties.end();
	uint32_t i;
	
	// avoid nested loop with lambda expression
	
	auto do_layer_names_match = [&i, &t_layer_names](const VkLayerProperties& t_other) {
		
		const char* layer = t_layer_names[i];
		return vk::are_strings_equal(layer, t_other.layerName);
	};
	
	// for each requested layer name, check layer properties for a match, if a match occurs, flag the result 
	
	VKCPP_FLAG result = VKCPP_NULL;
	for (i = 0; i < t_layer_count; ++i) {
		
		if (std::any_of(layer_properties_begin, layer_properties_end, do_layer_names_match)) {
			
			result |= VKCPP_INDEX[i];
		}
	}
	
	return result;
}

VKCPP_FLAG vk::are_instance_extensions_supported(const char* const* t_extension_names, uint32_t t_extension_count) {
	
	// get extension property count
	
	uint32_t extension_properties_count;
	VK_DEBUG
		( vkEnumerateInstanceExtensionProperties
		, "Failed to get instance extension count"
		, nullptr, &extension_properties_count, nullptr);
	
	// copy extension properties into usable memory
	
	std::vector<VkExtensionProperties> extension_properties{extension_properties_count};
	VK_DEBUG
		( vkEnumerateInstanceExtensionProperties
		, "Failed to get instance extension properties"
		, nullptr, &extension_properties_count, extension_properties.data());
	
	// store iterators
	
	auto extension_properties_begin = extension_properties.begin();
	auto extension_properties_end = extension_properties.end();
	uint32_t i;
	
	// avoid nested loop with lambda expression
	
	auto do_extension_names_match = [&i, &t_extension_names](const VkExtensionProperties& t_other) {
		
		const char* extension = t_extension_names[i];
		return are_strings_equal(extension, t_other.extensionName);
	};
	
	// for each requested extension name, check extension properties for a match, if a match occurs, flag the result 
	
	VKCPP_FLAG result = VKCPP_NULL;
	for (i = 0; i < t_extension_count; ++i) {
		
		if (std::any_of(extension_properties_begin, extension_properties_end, do_extension_names_match)) {
			
			result |= VKCPP_INDEX[i];
		}
	}
	
	return result;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback
	( VkDebugReportFlagsEXT t_flags
	, VkDebugReportObjectTypeEXT t_object_type
	, uint64_t t_object
	, size_t t_location
	, int32_t t_code
	, const char* t_layer_prefix
	, const char* t_message
	, void* t_user_data
	) {
	
	std::cout << "[VALIDATION-" << t_layer_prefix << "]: " << t_message << "\n";
	return VK_FALSE;
}

vk::instance::instance(const char* const* t_instance_layer_names, uint32_t t_instance_layer_count
	, const char* const* t_instance_extension_names, uint32_t t_instance_extension_count) {
	
	// check for support, throwing exceptions if unsuccessful
	
	VKCPP_FLAG result;
	result = are_instance_layers_supported(t_instance_layer_names, t_instance_layer_count);
	if (result != VKCPP_INDEX_SUM(t_instance_layer_count)) {
		std::cout << result << "\n";
		std::cout << t_instance_layer_count << "\n";
		std::cout << VKCPP_INDEX_SUM(t_instance_layer_count) << "\n";
		throw vk::vulkan_exception("One or more of the requested instance layers is not supported.");
	}
	
	result = are_instance_extensions_supported(t_instance_extension_names, t_instance_extension_count);
	if (result != VKCPP_INDEX_SUM(t_instance_extension_count)) {
		
		throw vk::vulkan_exception("One or more of the requested instance extensions is not supported.");
	}
	
	// continue with set up
	
	VkInstanceCreateInfo vk_instance_create_info = {};
	vk_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vk_instance_create_info.pNext = nullptr;
	vk_instance_create_info.flags = 0;
	vk_instance_create_info.pApplicationInfo = nullptr;
	vk_instance_create_info.enabledLayerCount = t_instance_layer_count;
	vk_instance_create_info.ppEnabledLayerNames = t_instance_layer_names;
	vk_instance_create_info.enabledExtensionCount = t_instance_extension_count;
	vk_instance_create_info.ppEnabledExtensionNames = t_instance_extension_names;
	
	VK_DEBUG
		( vkCreateInstance
		, "Failed to create instance"
		, &vk_instance_create_info, nullptr, &m_instance)
	
	// find physical devices
	
	uint32_t physical_device_count;
	VK_DEBUG
		( vkEnumeratePhysicalDevices
		, "Failed to get physical device count"
		, m_instance, &physical_device_count, nullptr)
	
	std::vector<VkPhysicalDevice> physical_devices{physical_device_count};
	VK_DEBUG
		( vkEnumeratePhysicalDevices
		, "Failed to get physical devices"
		, m_instance, &physical_device_count, physical_devices.data())
	
	m_physical_devices.resize(physical_device_count);
	for (uint32_t i = 0; i < physical_device_count; ++i) {
		
		m_physical_devices[i] = new vk::physical_device(physical_devices[i]);
	}
}

vk::instance::~instance() noexcept(false) {
	
	for (vk::physical_device* t_physical_device : m_physical_devices) {
		
		delete t_physical_device;
	}
	vkDestroyInstance(m_instance, nullptr);
}

VkInstance vk::instance::get_instance() const {
	
	return m_instance;
}

const std::vector<vk::physical_device*>& vk::instance::get_physical_devices() const {
	
	return m_physical_devices;
}

vk::validation::validation(vk::instance& t_instance) : m_instance{t_instance} {
	
	VkDebugReportCallbackCreateInfoEXT callback_info = {};
	callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callback_info.pNext = nullptr;
	callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callback_info.pfnCallback = debug_callback;
	callback_info.pUserData = nullptr;
	
	auto create_debug_report_callback = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(m_instance.get_instance(), "vkCreateDebugReportCallbackEXT");
	if (!create_debug_report_callback) {
		
		throw vk::vulkan_exception("Failed to get create debug report procedure address");
	}
	
	VK_DEBUG
		( create_debug_report_callback
		, "Failed to create debug report callback"
		, m_instance.get_instance(), &callback_info, nullptr, &m_callback)
}

vk::validation::~validation() noexcept(false) {
	
	auto destroy_debug_report_callback = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(m_instance.get_instance(), "vkDestroyDebugReportCallbackEXT");
	
	if (!destroy_debug_report_callback) {
		
		throw vk::vulkan_exception("Failed to get destroy debug report procedure address");
	}
	destroy_debug_report_callback(m_instance.get_instance(), m_callback, nullptr);
}

