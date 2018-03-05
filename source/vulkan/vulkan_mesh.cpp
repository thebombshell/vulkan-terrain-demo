
// vulkan_mesh.cpp
//
// source file for VKCPP mesh class
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_mesh.hpp"

//
// vertex_attribute
//

vk::vertex_attribute::vertex_attribute(const VkFormat& t_format, uint32_t t_offset) 
	: purpose{"!"}, format{t_format}, offset{t_offset} {
		
}

vk::vertex_attribute::vertex_attribute(const std::string& t_purpose, uint32_t t_offset) 
	: purpose{t_purpose}, format{vk::vertex_attribute::find_format(purpose)}, offset{t_offset} {
	
}

vk::vertex_attribute::~vertex_attribute() {
	
}

VkFormat vk::vertex_attribute::find_format(const std::string& t_purpose) {
	
	if (t_purpose == "POS") {
		
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	if (t_purpose == "COL") {
		
		return VK_FORMAT_R32G32B32A32_SFLOAT;
	}
	if (t_purpose == "NRM") {
		
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	if (t_purpose == "TAN") {
		
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	if (t_purpose == "BTN") {
		
		return VK_FORMAT_R32G32B32_SFLOAT;
	}
	if (t_purpose == "UV0") {
		
		return VK_FORMAT_R32G32_SFLOAT;
	}
	if (t_purpose == "UV1") {
		
		return VK_FORMAT_R32G32_SFLOAT;
	}
	if (t_purpose == "UV2") {
		
		return VK_FORMAT_R32G32_SFLOAT;
	}
	if (t_purpose == "UV3") {
		
		return VK_FORMAT_R32G32_SFLOAT;
	}
	return VK_FORMAT_UNDEFINED;
}

//
// vertex_binding
//

vk::vertex_binding::vertex_binding(uint32_t t_stride, const std::vector<vk::vertex_attribute>& t_attributes) 
	: stride{t_stride},  attributes{t_attributes} {
	
}

vk::vertex_binding::~vertex_binding() {
	
}

//
// vertex_definition
//

vk::vertex_definition::vertex_definition( const std::vector<vk::vertex_binding>& t_bindings ) : bindings{t_bindings} {
	
}

vk::vertex_definition::~vertex_definition() {
	
}