
// vulkan_mesh.cpp
//
// source file for VKCPP mesh class
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_mesh.hpp"

using namespace vk::vertex_purpose;

//
// vertex_attribute
//

vk::vertex_attribute::vertex_attribute(const VkFormat& t_format, uint32_t t_offset) 
	: purpose{VERR}, format{t_format}, offset{t_offset} {
	
	
}

vk::vertex_attribute::vertex_attribute(uint32_t t_purpose, uint32_t t_offset) 
	: purpose{t_purpose}, format{vk::vertex_attribute::find_format(purpose)}, offset{t_offset} {
	
	
}

vk::vertex_attribute::~vertex_attribute() {
	
	
}

VkFormat vk::vertex_attribute::find_format(uint32_t t_purpose) {
	
	switch (t_purpose) {
		
		case VUV0:
		case VUV1:
		case VUV2:
		case VUV3: {
			
			return VK_FORMAT_R32G32_SFLOAT;
		}
		break;
		
		case VPOS:
		case VNRM:
		case VTAN:
		case VBTN: {
			
			return VK_FORMAT_R32G32B32_SFLOAT;
		}
		break;
		
		case VCOL: {
			
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		break;
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