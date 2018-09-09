
// vulkan_image_view.cpp
//
// source file for the RAII wrapper of the VkImageView
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_image_view.hpp"
#include "vulkan_image.hpp"

vk::image_view::image_view(vk::device& t_device, vk::i_image& t_image, VkFormat t_format)
	: i_device_object{t_device}, m_image{t_image} {
	
	VkImageViewCreateInfo image_view_info = {};
	image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_info.pNext = nullptr;
	image_view_info.flags = 0;
	image_view_info.image = m_image.get_image();
	image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_info.format = t_format;
	image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_info.subresourceRange.baseMipLevel = 0;
	image_view_info.subresourceRange.levelCount = 1;
	image_view_info.subresourceRange.baseArrayLayer = 0;
	image_view_info.subresourceRange.layerCount = 1;
	
	VK_DEBUG
		( vkCreateImageView
		, "Failed to create swap chain image view"
		, m_device.get_device(), &image_view_info, nullptr, &m_image_view)
}

vk::image_view::image_view(vk::device& t_device, vk::i_image& t_image, VkFormat t_format, VkImageAspectFlags t_aspect_flags)
	: i_device_object{t_device}, m_image{t_image} {
	
	VkImageViewCreateInfo image_view_info = {};
	image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_info.pNext = nullptr;
	image_view_info.flags = 0;
	image_view_info.image = m_image.get_image();
	image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_info.format = t_format;
	image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.subresourceRange.aspectMask = t_aspect_flags;
	image_view_info.subresourceRange.baseMipLevel = 0;
	image_view_info.subresourceRange.levelCount = 1;
	image_view_info.subresourceRange.baseArrayLayer = 0;
	image_view_info.subresourceRange.layerCount = 1;
	
	VK_DEBUG
		( vkCreateImageView
		, "Failed to create swap chain image view"
		, m_device.get_device(), &image_view_info, nullptr, &m_image_view)
}

vk::image_view::~image_view() {
	
	vkDestroyImageView(m_device.get_device(), m_image_view, nullptr);
}

vk::i_image& vk::image_view::get_image() {
	
	return m_image;
}

const vk::i_image& vk::image_view::get_image() const {
	
	return m_image;
}

VkImageView vk::image_view::get_image_view() const {
	
	return m_image_view;
}