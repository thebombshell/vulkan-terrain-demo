
// vulkan_material.hpp
//
// header file for the VKCPP material class
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"

namespace vk {
	
	class material {
	
		public:
		
		material(const std::string& t_pipeline_name);
		~material();
		
		private:
		
		std::string m_pipeline_name;
		vk::buffer m_uniform_buffer;
	}
}