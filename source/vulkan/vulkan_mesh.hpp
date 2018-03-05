
// vulkan_mesh.hpp
//
// header file for VKCPP mesh class
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"
#include "vulkan_buffer.hpp"

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
		vertex_attribute(const vertex_attribute& t_other) = default;
		~vertex_attribute();
		
		static VkFormat find_format(const std::string& t_purpose);
		
		const std::string purpose;
		const VkFormat format;
		const uint32_t offset;
	};
	
	class vertex_binding {
		
		public:
		
		vertex_binding(uint32_t t_stride, const std::vector<vk::vertex_attribute>& t_attributes);
		vertex_binding(const vertex_binding& t_other) = default;
		~vertex_binding();
		
		const uint32_t stride;
		const std::vector<vk::vertex_attribute> attributes;
	};
	
	class vertex_definition {
		
		public:
		
		vertex_definition(const std::vector<vk::vertex_binding>& t_bindings);
		vertex_definition(const vertex_definition& t_other) = default;
		~vertex_definition();
		
		const std::vector<vk::vertex_binding> bindings;
	};
	
	class mesh : public i_device_object {
		
		public:
		
		mesh(vk::device& t_device, const vk::vertex_definition& t_vertex_definition, uint32_t t_index_count, uint32_t t_vertex_count);
		mesh(const mesh& t_other) = delete;
		virtual ~mesh();
		
		vk::staged_index_buffer& get_index_buffer();
		const vk::staged_index_buffer& get_index_buffer() const;
		const std::vector<vk::staged_vertex_buffer*>& get_vertex_buffers() const;
		
		protected:
		
		const vk::vertex_definition& m_vertex_definition;
		
		vk::staged_index_buffer m_index_buffer;
		std::vector<vk::staged_vertex_buffer*> m_vertex_buffers;
	};
}