
// /vulkan.hpp

#ifndef VK_TERRAIN_DEMO_VULKAN_HPP
#define VK_TERRAIN_DEMO_VULKAN_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#define VK_DEBUG(func, msg, ...) {VkResult result; if ((result = func( __VA_ARGS__ ))) { throw std::runtime_error( std::string("[VK_DEBUG]: ") + std::string(msg) + ": " + std::to_string(result)); } }

#define VULKAN_VALIDATION_LAYERS "VK_LAYER_LUNARG_standard_validation"
const static std::vector<std::string> g_validation_layer_strings{ VULKAN_VALIDATION_LAYERS };
const static std::vector<const char *> g_validation_layer_chars{ VULKAN_VALIDATION_LAYERS };
# define VULKAN_DEVICE_EXTENSIONS VK_KHR_SWAPCHAIN_EXTENSION_NAME
const static std::vector<std::string> g_device_extension_strings = { VULKAN_DEVICE_EXTENSIONS };
const static std::vector<const char*> g_device_extension_chars = { VULKAN_DEVICE_EXTENSIONS };
#define VULKAN_INSTANCE_EXTENSIONS VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME
const static std::vector<std::string> g_instance_extension_strings = { VULKAN_INSTANCE_EXTENSIONS };
const static std::vector<const char *> g_instance_extension_chars = { VULKAN_INSTANCE_EXTENSIONS };

namespace vk_terrain_demo {
	
	namespace vk {
		
		class context;
		class instance;
		class device;
		class surface;
		class swapchain;
		class graphics_pipeline;
	}
}

#endif