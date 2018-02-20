
// vulkan_exception

#include "vulkan.hpp"

#include <string>
#include <cstring>

bool vk::are_strings_equal(const char* t_a, const char* t_b) {
	
	return std::strcmp(t_a, t_b) == 0;
}

vk::vulkan_exception::vulkan_exception(const char* t_error, VkResult t_result) : m_result{t_result} {
	
	m_error = std::string("[VKCPP]: ") + t_error + ": " + std::to_string(t_result);
}
vk::vulkan_exception::vulkan_exception(const char* t_error) : m_error{"[VKCPP]: "}, m_result{VK_SUCCESS} {
	
	m_error = std::string("[VKCPP]: ") + t_error + ".";
}

vk::vulkan_exception::~vulkan_exception() {
	
}

const char* vk::vulkan_exception::what() const throw() {
	
	return m_error.c_str();
}