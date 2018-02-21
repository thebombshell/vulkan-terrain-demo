
// vulkan_asset_manager.hpp
//
// header file for the VKCPP asset manager
// graphics experiments easier and faster to produce
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_ASSET_MANAGER_HPP
#define VKCPP_VULKAN_ASSET_MANAGER_HPP

#include "vulkan.hpp"

#include <unordered_map>

namespace vk {
	
	const VKCPP_FLAG VKCPP_ASSET_DOES_NOT_EXIST{0};
	const VKCPP_FLAG VKCPP_ASSET_IS_USABLE{1};
	const VKCPP_FLAG VKCPP_ASSET_IS_UNUSABLE{2};
	
	class asset {
		
		public:
		
		template<typename T>
		asset() {
			
		}
		~asset();
		
		VKCPP_FLAG get_state();
		
		private:
		
		VKCPP_FLAG m_state;
		void* m_pointer;
	};
	
	class asset_manager {
		
		public:
		
		asset_manager(vk::context& t_context);
		~asset_manager();
		
		void add_asset(std::string t_name);
		void remove_asset(std::string t_name);
		VKCPP_FLAG get_asset_state();
		void get_asset();
		
		private:
		
		vk::context& m_context;
	};
}

#endif
