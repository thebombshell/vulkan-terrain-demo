
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
#include <mingw.mutex.h>
#include <unordered_map>

namespace vk {
	
	class i_locked_asset {
		
		public:
		
		i_locked_asset(i_locked_asset&&) = default;
		i_locked_asset(const i_locked_asset&) = delete;
		i_locked_asset& operator= (const i_locked_asset&) = delete;
		
		virtual ~i_locked_asset();
		
		protected:
		
		i_locked_asset(std::mutex& t_mutex);
		
		private:
		
		std::mutex& m_mutex;
	};
	
	class i_asset {
		
		public:
		
		virtual ~i_asset();
		virtual vk::i_locked_asset get_locked_asset() = 0;
		
		protected:
		
		i_asset();
		
		private:
		
	};
	
	template<typename T>
	class asset : public i_asset {
		
		public:
		
		class locked_asset : public vk::i_locked_asset {
			
			public:
			
			locked_asset(std::mutex& t_access_lock, T& t_asset) : i_locked_asset{t_access_lock}, m_asset{t_asset} {
				
			}
			
			locked_asset(const locked_asset&) = delete;
			locked_asset& operator= (const locked_asset&) = delete;
			
			~locked_asset() {
				
			}
			
			private:
			
			T& m_asset;
		};
		
		template<typename... T_ARGS>
		asset(T_ARGS... t_args) : m_asset{t_args...} {
		
		}
		
		virtual ~asset() {
		
		}
		
		vk::i_locked_asset get_locked_asset() {
			
			return locked_asset(m_access_lock, m_asset);
		}
		
		private:
		
		std::mutex m_access_lock;
		T m_asset;
	};
	
	class asset_manager {
		
		public:
		
		asset_manager(vk::context& t_context);
		~asset_manager();
		
		void create_shader(const std::string& t_name, VkShaderStageFlagBits t_shader_stage, const char* t_path);
		void create_pipeline(const std::string& t_name, std::vector<std::string> t_shader_names);
		void remove_asset(const std::string& t_name);
		vk::i_locked_asset get_asset(const std::string& t_name);
		
		private:
		
		vk::context& m_context;
		std::unordered_map<std::string, i_asset*> m_assets;
	};
}

#endif
