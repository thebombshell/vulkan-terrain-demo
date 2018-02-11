
// vulkan_image.hpp
//
// header file for the RAII wrapper of the VkImage
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_image.hpp"

vk::i_image::i_image() {
	
}

vk::i_image::~i_image() {
	
}

vk::image_reference::image_reference(VkImage t_image) : m_image{t_image} {
	
}

vk::image_reference::~image_reference() {
	
}

VkImage vk::image_reference::get_image() const {
	
	return m_image;
}

vk::image::image() {
	
}

vk::image::~image() {
	
}

VkImage vk::image::get_image() const {
	
	return m_image;
}