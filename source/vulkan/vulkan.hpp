
// vulkan.hpp
//
// master include for all vkcpp files, includes forward decclarations and widely reused debug classes and features.
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#ifndef VKCPP_VULKAN_HPP
#define VKCPP_VULKAN_HPP

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace vk {
	
	bool are_strings_equal(const char* t_a, const char* t_b);
	
	// frequently occuring result flags, usage depends on function
	
	#define VKCPP_FLAG unsigned int
	const VKCPP_FLAG VKCPP_NULL    { 0 };
	const VKCPP_FLAG VKCPP_INDEX_0 { 1 };
	const VKCPP_FLAG VKCPP_INDEX_1 { 2 };
	const VKCPP_FLAG VKCPP_INDEX_2 { 4 };
	const VKCPP_FLAG VKCPP_INDEX_3 { 8 };
	const VKCPP_FLAG VKCPP_INDEX_4 { 16 };
	const VKCPP_FLAG VKCPP_INDEX_5 { 32 };
	const VKCPP_FLAG VKCPP_INDEX_6 { 64 };
	const VKCPP_FLAG VKCPP_INDEX_7 { 128 };
	const VKCPP_FLAG VKCPP_INDEX_8 { 256 };
	const VKCPP_FLAG VKCPP_INDEX_9 { 512 };
	const VKCPP_FLAG VKCPP_INDEX_10{ 1024 };
	const VKCPP_FLAG VKCPP_INDEX_11{ 2048 };
	const VKCPP_FLAG VKCPP_INDEX_12{ 4096 };
	const VKCPP_FLAG VKCPP_INDEX_13{ 8192 };
	const VKCPP_FLAG VKCPP_INDEX_14{ 16384 };
	const VKCPP_FLAG VKCPP_INDEX_15{ 32768 };
	const VKCPP_FLAG VKCPP_INDEX_16{ 65536 };
	const VKCPP_FLAG VKCPP_INDEX_17{ 131072 };
	const VKCPP_FLAG VKCPP_INDEX_18{ 262144 };
	const VKCPP_FLAG VKCPP_INDEX_19{ 524288 };
	const VKCPP_FLAG VKCPP_INDEX_20{ 1048576 };
	const VKCPP_FLAG VKCPP_INDEX_21{ 2097152 };
	const VKCPP_FLAG VKCPP_INDEX_22{ 4194304 };
	const VKCPP_FLAG VKCPP_INDEX_23{ 8388608 };
	const VKCPP_FLAG VKCPP_INDEX_24{ 16777216 };
	const VKCPP_FLAG VKCPP_INDEX_25{ 33554432 };
	const VKCPP_FLAG VKCPP_INDEX_26{ 67108864 };
	const VKCPP_FLAG VKCPP_INDEX_27{ 134217728 };
	const VKCPP_FLAG VKCPP_INDEX_28{ 268435456 };
	const VKCPP_FLAG VKCPP_INDEX_29{ 536870912 };
	const VKCPP_FLAG VKCPP_INDEX_30{ 1073741824 };
	const VKCPP_FLAG VKCPP_INDEX_31{ 2147483648 };
	const VKCPP_FLAG VKCPP_INDEX[] = { VKCPP_INDEX_0, VKCPP_INDEX_1, VKCPP_INDEX_2, VKCPP_INDEX_3, VKCPP_INDEX_4
		, VKCPP_INDEX_5, VKCPP_INDEX_6, VKCPP_INDEX_7, VKCPP_INDEX_7, VKCPP_INDEX_8, VKCPP_INDEX_9, VKCPP_INDEX_10
		, VKCPP_INDEX_11, VKCPP_INDEX_12, VKCPP_INDEX_13, VKCPP_INDEX_14, VKCPP_INDEX_15, VKCPP_INDEX_16, VKCPP_INDEX_17
		, VKCPP_INDEX_18, VKCPP_INDEX_19, VKCPP_INDEX_20, VKCPP_INDEX_21, VKCPP_INDEX_22, VKCPP_INDEX_23, VKCPP_INDEX_24
		, VKCPP_INDEX_25, VKCPP_INDEX_26, VKCPP_INDEX_27, VKCPP_INDEX_28, VKCPP_INDEX_29, VKCPP_INDEX_30, VKCPP_INDEX_31};
	#define VKCPP_INDEX_SUM(x) (x >= 31 ? 4294967295 : VKCPP_INDEX[x] - 1)
	
	// vulkan_exception
	//
	// an exception specifically thrown for problems in called vulkan functions
	
	class vulkan_exception : public std::exception {
		
		public:
		
		vulkan_exception(const char* t_error, VkResult t_result);
		vulkan_exception(const char* t_error);
		~vulkan_exception();
		
		const char* what() const throw() override;
		
		private:
		
		std::string m_error;
		VkResult m_result;
	};
	
	// VK_DEBUG (func, msg, ...)
	//
	// given a function name (func), a const char error string (msg) and finally the functions arguments (...), checks for 
	// vulkan error results, if unsuccessful throws a vk::vulkan_exception with the message and the error result.
	
	#define VK_DEBUG(func, msg, ...) { VkResult vk_result_; if ((vk_result_ = func( __VA_ARGS__ ))) { throw vk::vulkan_exception(msg, vk_result_); } }
	
	// forward declarations
	
	class asset_manager;
	class buffer;
	class buffer_view;
	class command_pool;
	class command_buffer;
	class context;
	class descriptor_pool;
	class descriptor_set_layout;
	class device;
	class i_device_object;
	class display_mode;
	class display_plane_surface;
	class event;
	class fence;
	class forward_renderer;
	class framebuffer;
	class graphics_pipeline;
	class i_asset;
	class i_image;
	class i_locked_asset;
	class i_pipeline;
	class instance;
	class index_buffer;
	class image;
	class image_reference;
	class image_view;
	class instance;
	class pipeline_layout;
	class physical_device;
	class query_pool;
	class render_pass;
	class sampler;
	class scene;
	class semaphore;
	class shader_module;
	class shared_swapchain;
	class staged_buffer;
	class staged_index_buffer;
	class staged_vertex_buffer;
	class surface;
	class swapchain;
	class validation;
	class vertex_buffer;
}

#endif