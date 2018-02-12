
// vulkan_framebuffer.hpp
//
// header file for the RAII wrapper of the VkFrameBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan.hpp"

namespace vk {
	
	class framebuffer {
		
		public:
		
		framebuffer();
		~framebuffer();
		
		VkFrameBuffer get_framebuffer;
		
		private:
		
		VkFrameBuffer m_framebuffer;
	};
}