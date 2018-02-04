
// /vulkan_instance.cpp

#include "vulkan_instance.hpp"

#include <algorithm>
#include <iostream>

using namespace vk_terrain_demo;

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

vk::instance::instance() {
	
	create_instance();
	setup_validation();
}

vk::instance::~instance() {
	
	shutdown_validation();
	vkDestroyInstance(m_instance, nullptr);
}

VkInstance vk::instance::get_instance() {
	
	return m_instance;
}

void vk::instance::create_instance() {
	
	uint32_t extension_count;
	VK_DEBUG
		( vkEnumerateInstanceExtensionProperties
		, "Failed to get instance extension count"
		, nullptr, &extension_count, nullptr);
	
	std::vector<VkExtensionProperties> extension_properties{extension_count};
	VK_DEBUG
		( vkEnumerateInstanceExtensionProperties
		, "Failed to get instance extension properties"
		, nullptr, &extension_count, extension_properties.data());
	
	extension_count = 0;
	auto begin = g_instance_extension_strings.begin();
	auto end = g_instance_extension_strings.end();
	for (const VkExtensionProperties& t_property : extension_properties) {
		
		if (std::find(begin, end, std::string(t_property.extensionName)) != end) {
			
			++extension_count;
		}
	}
	
	if (extension_count != static_cast<uint32_t>(g_instance_extension_strings.size())) {
		
		throw std::runtime_error("Requested instance extensions not supported.");
	}
	
	VkInstanceCreateInfo vk_instance_create_info = {};
	vk_instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vk_instance_create_info.pNext = nullptr;
	vk_instance_create_info.flags = 0;
	vk_instance_create_info.pApplicationInfo = nullptr;
	vk_instance_create_info.enabledLayerCount = static_cast<uint32_t>(g_validation_layer_chars.size());
	vk_instance_create_info.ppEnabledLayerNames = g_validation_layer_chars.data();
	vk_instance_create_info.enabledExtensionCount = static_cast<uint32_t>(g_instance_extension_chars.size());
	vk_instance_create_info.ppEnabledExtensionNames = g_instance_extension_chars.data();
	
	VK_DEBUG
		( vkCreateInstance
		, "Failed to create instance"
		, &vk_instance_create_info, nullptr, &m_instance)
}

void vk::instance::setup_validation() {
	
	VkDebugReportCallbackCreateInfoEXT callback_info = {};
	callback_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callback_info.pNext = nullptr;
	callback_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callback_info.pfnCallback = debug_callback;
	callback_info.pUserData = nullptr;
	
	auto create_debug_report_callback = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
	if (!create_debug_report_callback) {
		
		throw std::runtime_error("Failed to get create debug report procedure address.");
	}
	
	VK_DEBUG
		( create_debug_report_callback
		, "Failed to create debug report callback"
		, m_instance, &callback_info, nullptr, &m_callback)
}

void vk::instance::shutdown_validation() {
	
	auto destroy_debug_report_callback = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
	
	if (!destroy_debug_report_callback) {
		
		throw std::runtime_error("Failed to get destroy debug report procedure address");
		destroy_debug_report_callback(m_instance, m_callback, nullptr);
	}
	destroy_debug_report_callback(m_instance, m_callback, nullptr);
}
