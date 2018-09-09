
// vulkan_physical_device.hpp
//
// header file for the set up object of the vk::device and RAII wrapper of the VkPhysicalDevice
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_PHYSICAL_DEVICE_HPP
#define VKCPP_VULKAN_PHYSICAL_DEVICE_HPP

#include "vulkan.hpp"

namespace vk {

	// vk::physical_device
	//
	// a setup class for the vk::device which includes a CPP style RAII compliant wrapper for the VkPhysicalDevice
	class physical_device {
		
		public:
		
		// constructor
		//
		// param - t_physical_device - the VkPhysicalDevice to wrap up
		physical_device(VkPhysicalDevice t_physical_device);
		
		// destructor
		~physical_device();
		
		// are_extensions_supported
		//
		// checks the physical device for extension compatibility setting flags relative to t_extension_names and returning 
		// them
		//
		// param - t_extension_names - an array of extension names to check for support
		// param - t_extension_count - the number of elements in t_extension_names
		// returns - flags relative to t_extension_names indicating which names are supported
		VKCPP_FLAG are_extensions_supported(const char* const* t_extension_names, int t_extension_count) const;
		
		// is_discrete_gpu
		//
		// returns - true if this physical device is a discrete gpu
		bool is_discrete_gpu() const;
		
		// find_memory_index
		//
		// checks for and returns a memory type index matching the required index flag and property flags
		//
		// param - t_index - a flag index presumably returned by a memory requirements query
		// param - t_property_flags - a set of property flags presumably returned by a memory requirements query
		// returns - the index of the matching memory type
		uint32_t find_memory_index(uint32_t t_index, VkMemoryPropertyFlags t_property_flags);
		
		// find_supported_format
		//
		// check for and returns an image format matching the given candidates and required features
		//
		// param - t_candidates - a list of acceptable formats to check support for, preferred format recommended first
		// param - t_features - a set of format feature flags describing required features for the format
		// param - t_tiling - optional image tiling flag, describing the required data ordering of the image
		// returns -
		VkFormat find_supported_format(const std::vector<VkFormat>& t_candidates, VkFormatFeatureFlags t_features, VkImageTiling t_tiling = VK_IMAGE_TILING_OPTIMAL);
		
		// has_feature
		//
		// returns - true if this physical device has the related feature
		
		bool has_robust_buffer_access() const;
		bool has_full_draw_index_uint32() const;
		bool has_image_cube_array() const;
		bool has_independent_blend() const;
		bool has_geometry_shader() const;
		bool has_tessellation_shader() const;
		bool has_sample_rate_shading() const;
		bool has_dual_src_blend() const;
		bool has_logic_op() const;
		bool has_multi_draw_indirect() const;
		bool has_draw_indirect_first_instance() const;
		bool has_depth_clamp() const;
		bool has_depth_bias_clamp() const;
		bool has_fill_mode_non_solid() const;
		bool has_depth_bounds() const;
		bool has_wide_lines() const;
		bool has_large_points() const;
		bool has_alpha_to_one() const;
		bool has_multi_viewport() const;
		bool has_sampler_anisotropy() const;
		bool has_texture_compression_ETC2() const;
		bool has_texture_compression_ASTC_LDR() const;
		bool has_texture_compression_BC() const;
		bool has_occlusion_query_precise() const;
		bool has_pipeline_statistics_query() const;
		bool has_vertex_pipeline_stores_and_atomics() const;
		bool has_fragment_stores_and_atomics() const;
		bool has_shader_tessellation_and_geometry_point_size() const;
		bool has_shader_image_gather_extended() const;
		bool has_shader_storage_image_extended_formats() const;
		bool has_shader_storage_image_multisample() const;
		bool has_shader_storage_image_read_without_format() const;
		bool has_shader_storage_image_write_without_format() const;
		bool has_shader_uniform_buffer_array_dynamic_indexing() const;
		bool has_shader_sampled_image_array_dynamic_indexing() const;
		bool has_shader_storage_buffer_array_dynamic_indexing() const;
		bool has_shader_storage_image_array_dynamic_indexing() const;
		bool has_shader_clip_distance() const;
		bool has_shader_cull_distance() const;
		bool has_shader_float64() const;
		bool has_shader_int64() const;
		bool has_shader_int16() const;
		bool has_shader_resource_residency() const;
		bool has_shader_resource_min_lod() const;
		bool has_sparse_binding() const;
		bool has_sparse_residency_buffer() const;
		bool has_sparse_residency_image2D() const;
		bool has_sparse_residency_image3D() const;
		bool has_sparse_residency_2_samples() const;
		bool has_sparse_residency_4_samples() const;
		bool has_sparse_residency_8_samples() const;
		bool has_sparse_residency_16_samples() const;
		bool has_sparse_residency_aliased() const;
		bool has_variable_multisample_rate() const;
		bool has_inherited_queries() const;
		
		// get_queue_family_properties
		//
		// returns - a collection of VkQueueFamilyProperties relating to this physical device
		const std::vector<VkQueueFamilyProperties>& get_queue_family_properties() const;
		
		// get_format_properties
		//
		VkFormatProperties get_format_properties(VkFormat t_format);
		
		// get_physical_device
		//
		// returns - the VkPhysicalDevice the class wraps up
		VkPhysicalDevice get_physical_device() const;
		
		private:
		
		VkPhysicalDevice m_physical_device;
		VkPhysicalDeviceProperties m_properties;
		VkPhysicalDeviceFeatures m_features;
		VkPhysicalDeviceMemoryProperties m_memory_properties;
		VkFormatProperties m_format_properties;
		std::vector<VkQueueFamilyProperties> m_queue_family_properties;
		std::vector<VkExtensionProperties> m_extension_properties;
		std::vector<VkSurfaceFormatKHR> m_surface_formats;
	};
}

#endif