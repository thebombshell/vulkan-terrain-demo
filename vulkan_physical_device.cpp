
// vulkan_physical_device.cpp
//
// source file for the set up object of the vk::device and RAII wrapper of the VkPhysicalDevice
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_physical_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_surface.hpp"

#include <algorithm>
#include <assert.h>
#include <cstring>
#include <stdexcept>

vk::physical_device::physical_device(VkPhysicalDevice t_physical_device) : m_physical_device{t_physical_device} {
	
	vkGetPhysicalDeviceProperties(m_physical_device, &m_properties);
	vkGetPhysicalDeviceFeatures(m_physical_device, &m_features);
	
	uint32_t queue_family_property_count;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_property_count, nullptr);
	m_queue_family_properties.resize(queue_family_property_count);
	vkGetPhysicalDeviceQueueFamilyProperties
		( m_physical_device, &queue_family_property_count, m_queue_family_properties.data());
	
	uint32_t extension_count;
	VK_DEBUG
		( vkEnumerateDeviceExtensionProperties
		, "Failed to get device extension count"
		, m_physical_device, nullptr, &extension_count, nullptr)
	m_extension_properties.resize(extension_count);
	VK_DEBUG
		( vkEnumerateDeviceExtensionProperties
		, "Failed to get device extension properties"
		, m_physical_device, nullptr, &extension_count, m_extension_properties.data())
}

vk::physical_device::~physical_device() {
	
}

VKCPP_FLAG vk::physical_device::are_extensions_supported(const char* const* t_extension_names, int t_extension_count) const {
	
	// store iterators
	
	auto extension_properties_begin = m_extension_properties.begin();
	auto extension_properties_end = m_extension_properties.end();
	int i;
	
	// avoid nested loop with lambda expression
	
	auto do_extension_names_match = [&i, &t_extension_names](const VkExtensionProperties& t_other) {
		
		const char* extension = t_extension_names[i];
		return vk::are_strings_equal(extension, t_other.extensionName);
	};
	
	// for each requested layer name, check layer properties for a match, if a match occurs, flag the result 
	
	VKCPP_FLAG result = VKCPP_NULL;
	for (i = 0; i < t_extension_count; ++i) {
		
		if (std::any_of(extension_properties_begin, extension_properties_end, do_extension_names_match)) {
			
			result |= VKCPP_INDEX[i];
		}
	}
	
	return result;
}

bool vk::physical_device::is_discrete_gpu() const {
	
	return m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

bool vk::physical_device::has_robust_buffer_access() const {
	
	return m_features.robustBufferAccess;
}
bool vk::physical_device::has_full_draw_index_uint32() const {
	
	return m_features.fullDrawIndexUint32;
}
bool vk::physical_device::has_image_cube_array() const {
	
	return m_features.imageCubeArray;
}
bool vk::physical_device::has_independent_blend() const {
	
	return m_features.independentBlend;
}
bool vk::physical_device::has_geometry_shader() const {
	
	return m_features.geometryShader;
}
bool vk::physical_device::has_tessellation_shader() const {
	
	return m_features.tessellationShader;
}
bool vk::physical_device::has_sample_rate_shading() const {
	
	return m_features.sampleRateShading;
}
bool vk::physical_device::has_dual_src_blend() const {
	
	return m_features.dualSrcBlend;
}
bool vk::physical_device::has_logic_op() const {
	
	return m_features.logicOp;
}
bool vk::physical_device::has_multi_draw_indirect() const {
	
	return m_features.multiDrawIndirect;
}
bool vk::physical_device::has_draw_indirect_first_instance() const {
	
	return m_features.drawIndirectFirstInstance;
}
bool vk::physical_device::has_depth_clamp() const {
	
	return m_features.depthClamp;
}
bool vk::physical_device::has_depth_bias_clamp() const {
	
	return m_features.depthBiasClamp;
}
bool vk::physical_device::has_fill_mode_non_solid() const {
	
	return m_features.fillModeNonSolid;
}
bool vk::physical_device::has_depth_bounds() const {
	
	return m_features.depthBounds;
}
bool vk::physical_device::has_wide_lines() const {
	
	return m_features.wideLines;
}
bool vk::physical_device::has_large_points() const {
	
	return m_features.largePoints;
}
bool vk::physical_device::has_alpha_to_one() const {
	
	return m_features.alphaToOne;
}
bool vk::physical_device::has_multi_viewport() const {
	
	return m_features.multiViewport;
}
bool vk::physical_device::has_sampler_anisotropy() const {
	
	return m_features.samplerAnisotropy;
}
bool vk::physical_device::has_texture_compression_ETC2() const {
	
	return m_features.textureCompressionETC2;
}
bool vk::physical_device::has_texture_compression_ASTC_LDR() const {
	
	return m_features.textureCompressionASTC_LDR;
}
bool vk::physical_device::has_texture_compression_BC() const {
	
	return m_features.textureCompressionBC;
}
bool vk::physical_device::has_occlusion_query_precise() const {
	
	return m_features.occlusionQueryPrecise;
}
bool vk::physical_device::has_pipeline_statistics_query() const {
	
	return m_features.pipelineStatisticsQuery;
}
bool vk::physical_device::has_vertex_pipeline_stores_and_atomics() const {
	
	return m_features.vertexPipelineStoresAndAtomics;
}
bool vk::physical_device::has_fragment_stores_and_atomics() const {
	
	return m_features.fragmentStoresAndAtomics;
}
bool vk::physical_device::has_shader_tessellation_and_geometry_point_size() const {
	
	return m_features.shaderTessellationAndGeometryPointSize;
}
bool vk::physical_device::has_shader_image_gather_extended() const {
	
	return m_features.shaderImageGatherExtended;
}
bool vk::physical_device::has_shader_storage_image_extended_formats() const {
	
	return m_features.shaderStorageImageExtendedFormats;
}
bool vk::physical_device::has_shader_storage_image_multisample() const {
	
	return m_features.shaderStorageImageMultisample;
}
bool vk::physical_device::has_shader_storage_image_read_without_format() const {
	
	return m_features.shaderStorageImageReadWithoutFormat;
}
bool vk::physical_device::has_shader_storage_image_write_without_format() const {
	
	return m_features.shaderStorageImageWriteWithoutFormat;
}
bool vk::physical_device::has_shader_uniform_buffer_array_dynamic_indexing() const {
	
	return m_features.shaderUniformBufferArrayDynamicIndexing;
}
bool vk::physical_device::has_shader_sampled_image_array_dynamic_indexing() const {
	
	return m_features.shaderSampledImageArrayDynamicIndexing;
}
bool vk::physical_device::has_shader_storage_buffer_array_dynamic_indexing() const {
	
	return m_features.shaderStorageBufferArrayDynamicIndexing;
}
bool vk::physical_device::has_shader_storage_image_array_dynamic_indexing() const {
	
	return m_features.shaderStorageImageArrayDynamicIndexing;
}
bool vk::physical_device::has_shader_clip_distance() const {
	
	return m_features.shaderClipDistance;
}
bool vk::physical_device::has_shader_cull_distance() const {
	
	return m_features.shaderCullDistance;
}
bool vk::physical_device::has_shader_float64() const {
	
	return m_features.shaderFloat64;
}
bool vk::physical_device::has_shader_int64() const {
	
	return m_features.shaderInt64;
}
bool vk::physical_device::has_shader_int16() const {
	
	return m_features.shaderInt16;
}
bool vk::physical_device::has_shader_resource_residency() const {
	
	return m_features.shaderResourceResidency;
}
bool vk::physical_device::has_shader_resource_min_lod() const {
	
	return m_features.shaderResourceMinLod;
}
bool vk::physical_device::has_sparse_binding() const {
	
	return m_features.sparseBinding;
}
bool vk::physical_device::has_sparse_residency_buffer() const {
	
	return m_features.sparseResidencyBuffer;
}
bool vk::physical_device::has_sparse_residency_image2D() const {
	
	return m_features.sparseResidencyImage2D;
}
bool vk::physical_device::has_sparse_residency_image3D() const {
	
	return m_features.sparseResidencyImage3D;
}
bool vk::physical_device::has_sparse_residency_2_samples() const {
	
	return m_features.sparseResidency2Samples;
}
bool vk::physical_device::has_sparse_residency_4_samples() const {
	
	return m_features.sparseResidency4Samples;
}
bool vk::physical_device::has_sparse_residency_8_samples() const {
	
	return m_features.sparseResidency8Samples;
}
bool vk::physical_device::has_sparse_residency_16_samples() const {
	
	return m_features.sparseResidency16Samples;
}
bool vk::physical_device::has_sparse_residency_aliased() const {
	
	return m_features.sparseResidencyAliased;
}
bool vk::physical_device::has_variable_multisample_rate() const {
	
	return m_features.variableMultisampleRate;
}
bool vk::physical_device::has_inherited_queries() const {
	
	return m_features.inheritedQueries;
}

const std::vector<VkQueueFamilyProperties>& vk::physical_device::get_queue_family_properties() const {
	
	return m_queue_family_properties;
}

VkPhysicalDevice vk::physical_device::get_physical_device() const {
	
	return m_physical_device;
}
