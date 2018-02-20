
// vulkan_command_buffer.cpp
//
// source file for the RAII wrrapper of the VkCommandBuffer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_buffer.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_render_pass.hpp"

vk::command_buffer::command_buffer(vk::device& t_device, vk::command_pool& t_command_pool) 
	: device_object{t_device}, m_command_pool{t_command_pool} {
	
	VkCommandBufferAllocateInfo command_buffer_info = {};
	command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_info.pNext = nullptr;
	command_buffer_info.commandPool = m_command_pool.get_command_pool();
	command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_info.commandBufferCount = 1;
	VK_DEBUG
		( vkAllocateCommandBuffers
		, "Failed to allocate command buffers"
		, m_device.get_device(), &command_buffer_info, &m_command_buffer)
}

vk::command_buffer::~command_buffer() {
	
}

VkCommandBuffer vk::command_buffer::get_command_buffer() {
	
	return m_command_buffer;
}

void vk::command_buffer::begin() {
	
	VkCommandBufferBeginInfo command_buffer_begin_info = {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.pNext = nullptr;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	command_buffer_begin_info.pInheritanceInfo = nullptr;
	
	VK_DEBUG
		( vkBeginCommandBuffer
		, "Failed to begin command buffer"
		, m_command_buffer, &command_buffer_begin_info)
}

void vk::command_buffer::begin_render_pass
	( vk::render_pass& t_render_pass, vk::framebuffer& t_framebuffer
	, VkRect2D t_render_area, const VkClearValue* const t_clear_values, uint32_t t_clear_value_count) {
	
	VkRenderPassBeginInfo render_pass_begin_info = {};
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.pNext = nullptr;
	render_pass_begin_info.renderPass = t_render_pass.get_render_pass();
	render_pass_begin_info.framebuffer = t_framebuffer.get_framebuffer();
	render_pass_begin_info.renderArea = t_render_area;
	render_pass_begin_info.clearValueCount = t_clear_value_count;
	render_pass_begin_info.pClearValues = t_clear_values;
	vkCmdBeginRenderPass(m_command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void vk::command_buffer::bind_pipeline(vk::pipeline& t_pipeline) {
	
	vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, t_pipeline.get_pipeline());
}

void vk::command_buffer::bind_vertex_buffers(std::vector<vk::buffer*>& t_buffers) {
	
	std::vector<VkBuffer> buffers {t_buffers.size()};
	std::vector<VkDeviceSize> offsets {t_buffers.size()};
	for (int i = 0; i < t_buffers.size(); i++) {
		
		buffers[i] = t_buffers[i]->get_buffer();
		offsets[i] = 0;
	}
	vkCmdBindVertexBuffers(m_command_buffer, 0, static_cast<uint32_t>(buffers.size()), buffers.data(), offsets.data());
}

void vk::command_buffer::bind_index_buffers(vk::buffer& t_buffer, VkIndexType t_index_type) {
	
	vkCmdBindIndexBuffer(m_command_buffer, t_buffer.get_buffer(), 0, t_index_type);
}

void vk::command_buffer::copy_staged_buffer(vk::staged_buffer& t_buffer) {
	
	VkBufferCopy buffer_copy = {};
	buffer_copy.srcOffset = 0;
	buffer_copy.dstOffset = 0;
	buffer_copy.size = size;
	vkCmdCopyBuffer(m_command_buffer, t_buffer.get_staging_buffer(), t_buffer.get_buffer(), 1, &buffer_copy);
}

void vk::command_buffer::draw(uint32_t t_vertex_count, uint32_t t_instance_count, uint32_t t_vertex_offset, uint32_t t_instance_offset) {
	
	vkCmdDraw(m_command_buffer, t_vertex_count, t_instance_count, t_vertex_offset, t_instance_offset);
}

void vk::command_buffer::end_render_pass() {
	
	vkCmdEndRenderPass(m_command_buffer);
}

void vk::command_buffer::end() {
	
	VK_DEBUG
		( vkEndCommandBuffer
		, "Failed to end command buffer"
		, m_command_buffer)
}