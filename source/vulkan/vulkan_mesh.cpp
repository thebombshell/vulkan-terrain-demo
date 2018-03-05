
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

//
// mesh
//

vk::mesh::mesh
	( vk::device& t_device
	, const vk::vertex_definition& t_vertex_definition
	, uint32_t t_index_count, uint32_t t_vertex_count) 
	: i_device_object{t_device}, m_vertex_definition{t_vertex_definition}
	, m_index_buffer{t_device, t_index_count}, m_vertex_buffers{t_vertex_definition.bindings.size()} {
	
	for (uint32_t i = 0; i < t_vertex_definition.bindings.size(); ++i) {
		
		m_vertex_buffers[i] = new vk::staged_vertex_buffer{t_device, t_vertex_definition.bindings[i].stride * t_vertex_count};
	}
}

vk::mesh::~mesh() {
	
	for (auto* vertex_buffer : m_vertex_buffers) {
		
		delete vertex_buffer;
	}
}

vk::staged_index_buffer& vk::mesh::get_index_buffer() {
	
	return m_index_buffer;
}

const vk::staged_index_buffer& vk::mesh::get_index_buffer() const {
	
	return m_index_buffer;
}

const std::vector<vk::staged_vertex_buffer*>& vk::mesh::get_vertex_buffers() const {
	
	return m_vertex_buffers;
}
