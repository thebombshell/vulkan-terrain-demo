
// vulkan_mesh.hpp
//
// header file for VKCPP mesh helper classes
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"
#include "vulkan_buffer.hpp"

namespace vk {
	
	namespace vertex_purpose {
		
		static const uint32_t VERR {0x00000000};
		static const uint32_t VPOS {0x00000001};
		static const uint32_t VCOL {0x00000002};
		static const uint32_t VNRM {0x00000003};
		static const uint32_t VTAN {0x00000004};
		static const uint32_t VBTN {0x00000005};
		static const uint32_t VUV0 {0x00000006};
		static const uint32_t VUV1 {0x00000007};
		static const uint32_t VUV2 {0x00000008};
		static const uint32_t VUV3 {0x0000000a};
	};
	
	class vertex_attribute {
		
		public:
		
		vertex_attribute(const VkFormat& t_format, uint32_t t_offset);
		vertex_attribute(uint32_t t_purpose, uint32_t t_offset);
		vertex_attribute(const vertex_attribute& t_other) = default;
		~vertex_attribute();
		
		static VkFormat find_format(uint32_t t_purpose);
		
		const uint32_t purpose;
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
	
}