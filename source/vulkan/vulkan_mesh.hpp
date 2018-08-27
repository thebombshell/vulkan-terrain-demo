
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
	
	namespace vertex {
		
		struct i_vertex {
			
			public:
			
			virtual ~i_vertex() = 0;
			
			virtual const vk::vertex_definition& get_definition() const = 0;
			
			protected:
			
			i_vertex() = default;
		};
		
		struct i_pos_vertex : public virtual i_vertex {
			
			public:
			
			virtual ~i_pos_vertex() = 0;
			
			virtual float* get_position() = 0;
			virtual const float* get_position() const = 0;
			
			protected:
			
			i_pos_vertex() = default;
			
		};
		
		struct i_col_vertex : public virtual i_vertex {
			
			public:
			
			virtual ~i_col_vertex() = 0;
			
			virtual float* get_color() = 0;
			virtual const float* get_color() const = 0;
			
			protected:
			
			i_col_vertex() = default;
			
		};
		
		struct i_nrm_vertex : public virtual i_vertex {
			
			public:
			
			virtual ~i_nrm_vertex() = 0;
			
			virtual float* get_normal() = 0;
			virtual const float* get_normal() const = 0;
			
			protected:
			
			i_nrm_vertex() = default;
			
		};
		
		struct pos_col_nrm : public i_pos_vertex, public i_col_vertex, public i_nrm_vertex {
			
			public:
			
			pos_col_nrm
				( float t_pos_x, float t_pos_y, float t_pos_z
				, float t_col_r, float t_col_g, float t_col_b, float t_col_a
				, float t_nrm_x, float t_nrm_y, float t_nrm_z );
			pos_col_nrm();
			~pos_col_nrm();
			
			float* get_position() override;
			const float* get_position() const override;
			
			float* get_color() override;
			const float* get_color() const override;
			
			float* get_normal() override;
			const float* get_normal() const override;
			
			const vk::vertex_definition& get_definition() const override;
			static const vk::vertex_definition& get_vertex_definition();
			
			float data[10];
		};
		
	}
	
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