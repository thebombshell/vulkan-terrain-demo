
// vulkan_asset_manager.cpp
//
// source file for the VKCPP asset manager
// graphics experiments easier and faster to produce
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_asset_manager.hpp"
#include "vulkan_context.hpp"
#include "vulkan_shader_module.hpp"

//
// i_asset
//

vk::i_asset::~i_asset() {
	
}

vk::i_asset::i_asset() {
	
}

//
// i_locked_asset
//

vk::i_locked_asset::i_locked_asset(std::mutex& t_mutex) : m_mutex{t_mutex} {
	
	t_mutex.lock();
}

vk::i_locked_asset::~i_locked_asset() {
	
	m_mutex.unlock();
}

//
// asset_manager
//

vk::asset_manager::asset_manager(vk::context& t_context) : m_context{t_context} {
	
	
}

vk::asset_manager::~asset_manager() {
	
	
}

void vk::asset_manager::create_shader(const std::string& t_name, VkShaderStageFlagBits t_shader_stage, const char* t_path) {
	
	auto iterator = m_assets.find(t_name);
	if (iterator != m_assets.end()) {
		
		throw std::runtime_error("An asset by this name already exists.");
	}
	m_assets[t_name] = new asset<vk::shader_module>(m_context.get_device(), t_shader_stage, t_path);
}

void vk::asset_manager::remove_asset(const std::string& t_name) {
	
	auto iterator = m_assets.find(t_name);
	if (iterator == m_assets.end()) {
		
		throw std::runtime_error("An asset by this name does not exist.");
	}
	delete iterator->second;
	m_assets.erase(iterator);
}

vk::i_locked_asset vk::asset_manager::get_asset(const std::string& t_name) {
	
	auto iterator = m_assets.find(t_name);
	if (iterator == m_assets.end()) {
		
		throw std::runtime_error("An asset by this name does not exist.");
	}
	return iterator->second->get_locked_asset();
}