
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

vk::vertex::i_vertex::~i_vertex() {
	
}


vk::vertex::i_pos_vertex::~i_pos_vertex() {
	
}

vk::vertex::i_col_vertex::~i_col_vertex() {
	
}

vk::vertex::i_nrm_vertex::~i_nrm_vertex() {
	
}

vk::vertex::pos_col_nrm::pos_col_nrm
	( float t_pos_x, float t_pos_y, float t_pos_z
	, float t_col_r, float t_col_g, float t_col_b, float t_col_a
	, float t_nrm_x, float t_nrm_y, float t_nrm_z ) : data
		{ t_pos_x, t_pos_y, t_pos_z
		, t_col_r, t_col_g, t_col_b, t_col_a
		, t_nrm_x, t_nrm_y, t_nrm_z } {
	
}

vk::vertex::pos_col_nrm::pos_col_nrm(): data
		{ 0.0f, 0.0f, 0.0f
		, 1.0f, 1.0f, 1.0f, 1.0f
		, 0.0f, 0.0f, 1.0f } {
}

vk::vertex::pos_col_nrm::~pos_col_nrm() {
}

float* vk::vertex::pos_col_nrm::get_position() {
	
	return &data[0];
}

const float* vk::vertex::pos_col_nrm::get_position() const {
	
	return &data[0];
}

float* vk::vertex::pos_col_nrm::get_color() {
	
	return &data[3];
}

const float* vk::vertex::pos_col_nrm::get_color() const {
	
	return &data[3];
}

float* vk::vertex::pos_col_nrm::get_normal() {
	
	return &data[7];
}

const float* vk::vertex::pos_col_nrm::get_normal() const  {
	
	return &data[7];
}

const vk::vertex_definition& vk::vertex::pos_col_nrm::get_definition() const {
	
	return vk::vertex::pos_col_nrm::get_vertex_definition();
}

const vk::vertex_definition& vk::vertex::pos_col_nrm::get_vertex_definition() {
	
	static const std::vector<vk::vertex_attribute> attributes{{VPOS, 0}, {VCOL, sizeof(float) * 3}, {VNRM, sizeof(float) * 7}};
	static const std::vector<vk::vertex_binding> bindings {{sizeof(float) * 10, attributes}};
	static const vk::vertex_definition definition{ bindings };
	return definition;
}

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