
// vulkan_forward_renderer.hpp
//
// source file for the multi-threaded forward renderer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"

namespace vk {
	
	class mesh_instance {
	
		public:
		
		~instance();
		instance();
		
		private:
		
		std::string& m_mesh_name;
		std::string& m_material_name;
		float m_world_matrix[16];
		vk::buffer m_uniform_buffer;
	};
	
	class scene {
		
		public:
		
		scene();
		~scene();
		
		vk::mesh_instance& create_mesh_instance(const std::string& m_name);
		
		private:
		
		std::vector<vk::mesh_instance*> m_instances;
	};
	
	class forward_renderer {
		
		public:
		
		forward_renderer(vk::asset_manager& t_asset_manager);
		~forward_renderer();
		
		void render();
		
		private:
		
		vk::asset_manager& m_asset_manager;
		vk::scene m_scene;
	};
}