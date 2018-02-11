
// vulkan_instance.hpp
//
// header file for the RAII wrapper of the VkInstance
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_INSTANCE_HPP
#define VKCPP_VULKAN_INSTANCE_HPP

#include "vulkan.hpp"

namespace vk {
	
	// are_instance_layers_supported
	//
	// checks for and returns instance layer support
	//
	// param - t_layer_names - an array of instance layer names to check for support
	// param - t_layer_count - the number of instance layer names in t_layer_names
	// returns - a VKCPP_FLAG with indices flagged relative to t_layer_names to indicate support
	VKCPP_FLAG are_instance_layers_supported(const char* const* t_layer_names, uint32_t t_layer_count);
	
	// are_instance_extensions_supported
	//
	// checks for and returns instance extensions support
	//
	// param - t_extension_names - an array of instance extension names to check for support
	// param - t_extension_count - the number of instance extension names in t_extension_names
	// returns - a VKCPP_FLAG with indices flagged relative to t_layer_names to indicate support
	VKCPP_FLAG are_instance_extensions_supported(const char* const* t_extension_names, uint32_t t_extension_count);
	
	// vk::instance
	//
	// an CPP style RAII compliant wrapper for the VkInstance
	class instance {
		
		public:
		
		// constructor
		//
		// param - t_layer_names - an array of instance layer names to instantiate
		// param - t_layer_count - the number of instance layer names in t_layer_names
		// param - t_extension_names - an array of instance extension names to check for support
		// param - t_extension_count - the number of instance extension names in t_extension_names
		instance(const char* const* t_layer_names, uint32_t t_layer_count, const char* const* t_extension_names, uint32_t t_extension_count);
		
		// deconstructor
		~instance() noexcept(false);
		
		// get_instance
		//
		// returns - a copy of the VkInstance
		VkInstance get_instance() const;
		
		// get_physical_devices
		//
		// returns - a collection of vkcpp physical devices which may be queried for feature support
		const std::vector<vk::physical_device*>& get_physical_devices() const;
		
		private:
		
		VkInstance m_instance;
		std::vector<vk::physical_device*> m_physical_devices;
	};
	
	// vk::validation
	//
	// a CPP style RAII compliant wrapper for validation layer functionality in the VkInstance
	class validation {
		
		public:
		
		// constructor
		//
		// param - t_instance - the vk::instance to register the validation callback to
		validation(vk::instance& t_instance);
		
		// destructor
		~validation() noexcept(false);
		
		private:
		
		vk::instance& m_instance;
		VkDebugReportCallbackEXT m_callback;
	};
}

#endif