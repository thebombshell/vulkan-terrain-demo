
// vulkan_mesh.hpp
//
// header file for VKCPP mesh class
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"

namespace vk {
	
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_ERROR{"!"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_POSITION{"POS"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_COLOR{"COL"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_NORMAL{"NRM"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_TANGENT{"TAN"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_BITANGENT{"BTN"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_TEXCOORD0{"UV0"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_TEXCOORD1{"UV1"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_TEXCOORD2{"UV2"};
	const std::string VKCPP_VERTEX_ATTRIBUTE_NAME_TEXCOORD3{"UV3"};
	
	class vertex_attribute {
		
		public:
		
		vertex_attribute(const VkFormat& t_format, uint32_t t_offset);
		vertex_attribute(const std::string& t_purpose, uint32_t t_offset);
		~vertex_attribute();
		
		static VkFormat find_format(const std::string& t_purpose);
		
		const std::string purpose;
		const VkFormat format;
		const uint32_t offset;
	};
	
	class vertex_binding {
		
		public:
		
		vertex_binding(uint32_t t_stride, const std::vector<vk::vertex_attribute>& t_attributes);
		~vertex_binding();
		
		const uint32_t stride;
		const std::vector<vk::vertex_attribute> attributes;
	};
	
	class vertex_definition {
		
		public:
		
		vertex_definition( const std::vector<vk::vertex_binding>& t_bindings );
		~vertex_definition();
		
		const std::vector<vk::vertex_binding> bindings;
	};
	/*
	class mesh {
		
		public:
		
		private:
		
		vk::vertex_definition& m_vertex_definition;
		
		vk::buffer& m_index_buffer;
		vk::buffer& m_vertex_buffers;
	};*/
}