
// vulkan_image.hpp
//
// header file for the RAII wrapper of the VkImage
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_image.hpp"
#include "vulkan_physical_device.hpp"

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

vk::image::image(vk::device& t_device, VkImageType t_type, VkFormat t_format, VkExtent3D t_extent, VkImageCreateFlags t_flags
	, uint32_t t_mip_levels, uint32_t t_array_layers, VkSampleCountFlagBits t_sample_count, VkImageTiling t_tiling
	, VkImageLayout t_layout, VkImageUsageFlags t_usage, VkSharingMode t_sharing_mode, std::vector<uint32_t>* t_queues)
	: i_device_object{t_device} {
	
	VkImageCreateInfo image_info =
		{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
		, nullptr
		, t_flags
		, t_type
		, t_format
		, t_extent
		, t_mip_levels
		, t_array_layers
		, t_sample_count
		, t_tiling
		, t_usage
		, t_sharing_mode
		, t_queues ? static_cast<uint32_t>(t_queues->size()) : 0
		, t_queues ? &(*t_queues)[0] : nullptr
		, t_layout
		};
	
	VK_DEBUG
		( vkCreateImage
		, "failed to create image"
		, m_device.get_device(), &image_info, nullptr, &m_image)
	
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(m_device.get_device(), m_image, &memory_requirements);
	
	VkMemoryAllocateInfo memory_alocate_info = {};
	memory_alocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_alocate_info.pNext = nullptr;
	memory_alocate_info.allocationSize = memory_requirements.size;
	memory_alocate_info.memoryTypeIndex = m_device.get_physical_device().find_memory_index
		( memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
	
	VK_DEBUG
		( vkAllocateMemory
		, "Failed to allocate memory"
		, m_device.get_device(), &memory_alocate_info, nullptr, &m_device_memory)
	
	VK_DEBUG
		( vkBindImageMemory
		, "failed to bind image memory"
		, m_device.get_device(), m_image, m_device_memory, 0)
}

vk::image::~image() {
	
}

VkImage vk::image::get_image() const {
	
	return m_image;
}

//
// depth_image
//

const std::vector<VkFormat>& vk::get_depth_formats() {
	
	static std::vector<VkFormat> formats
		{ VK_FORMAT_D32_SFLOAT_S8_UINT
		, VK_FORMAT_D24_UNORM_S8_UINT
		, VK_FORMAT_D16_UNORM_S8_UINT
		};
	return formats;
}

vk::depth_image::depth_image(vk::device& t_device, VkExtent3D t_extent) 
	: image{ t_device, VK_IMAGE_TYPE_2D
	, t_device.get_physical_device().find_supported_format(vk::get_depth_formats(), VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL)
	, t_extent, 0, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
	, VK_SHARING_MODE_EXCLUSIVE, nullptr } {
	
}

vk::depth_image::depth_image(vk::device& t_device, VkFormat t_format, VkExtent3D t_extent) 
	: image{ t_device, VK_IMAGE_TYPE_2D, t_format, t_extent, 0, 1, 1
	, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_UNDEFINED
	, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, nullptr } {
	
}

vk::depth_image::~depth_image() {
	
}