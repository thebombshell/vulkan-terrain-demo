
// vulkan_context.cpp
//
// source file for the VKCPP all encompassing graphical context, intended to contain complex vulkan procedures and make
// graphics experiments easier and faster to produce
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_buffer.hpp"
#include "vulkan_context.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_surface.hpp"
#include "vulkan_swapchain.hpp"

#include <algorithm>
#include <iostream>

vk::context::context(HWND t_window_handle, HINSTANCE t_handle_instance)
	: m_instance{nullptr}, m_surface{nullptr}, m_device{nullptr}, m_swapchain{nullptr}, m_pipeline{nullptr}, m_command_pool{nullptr} {
	
	const char* const instance_layers[] = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	const char* const instance_extensions[] = {
		"VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};
	const char* const device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	m_instance = new vk::instance( instance_layers, 1, instance_extensions, 3);
	m_validation = new vk::validation(*m_instance);
	for (auto* t_physical_device : m_instance->get_physical_devices()) {
		
		if ( t_physical_device->is_discrete_gpu() 
			&& t_physical_device->has_geometry_shader()
			&& t_physical_device->are_extensions_supported(device_extensions, 1)) {
			
			m_surface = new vk::surface(t_window_handle, t_handle_instance, *m_instance, *t_physical_device);
			m_device = new vk::device(*t_physical_device, *m_surface, nullptr, 0, device_extensions, 1);
			break;
		}
	}
	m_swapchain = new vk::swapchain(*m_surface, *m_device);
	m_pipeline = new vk::graphics_pipeline(*m_swapchain);
	m_vertex_buffer = new vk::vertex_buffer(*m_device, sizeof(float) * 6 * 3);
	std::vector<float> vertex_data = 
		{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f
		, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f
		, 0.0f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
	m_vertex_buffer->map(vertex_data.data(), static_cast<uint32_t>(sizeof(float) * vertex_data.size()));
	std::vector<vk::buffer*> buffers = {m_vertex_buffer};
	m_command_pool = new vk::command_pool(*m_device, m_device->get_graphical_queue_family_index());
	for (auto* t_image_view : m_swapchain->get_image_views()) {
		
		std::vector<vk::image_view*> image_views = {t_image_view};
		vk::framebuffer* framebuffer = new vk::framebuffer(*m_device, m_pipeline->get_render_pass(), image_views, m_swapchain->get_extent(), 1);
		m_framebuffers.push_back( framebuffer );
		vk::command_buffer* command_buffer = new vk::command_buffer(*m_device, *m_command_pool);
		m_command_buffers.push_back( command_buffer );
		command_buffer->begin();
		VkRect2D render_area = {};
		render_area.offset = {0, 0};
		render_area.extent = m_swapchain->get_extent();
		VkClearValue value = {0.0f, 0.0f, 0.0f, 1.0f};
		command_buffer->begin_render_pass( m_pipeline->get_render_pass(), *framebuffer, render_area, &value, 1);
		command_buffer->bind_pipeline(*m_pipeline);
		command_buffer->bind_vertex_buffers(buffers);
		command_buffer->draw(3, 1, 0, 0);
		command_buffer->end_render_pass();
		command_buffer->end();
	}
	m_image_available_semaphore = new vk::semaphore(*m_device);
	m_render_finished_semaphore = new vk::semaphore(*m_device);
}

vk::context::~context() {
	
	vkDeviceWaitIdle(m_device->get_device());
	
	if (m_render_finished_semaphore) {
		
		delete m_render_finished_semaphore;
	}
	if (m_image_available_semaphore) {
		
		delete m_image_available_semaphore;
	}
	for (auto* t_command_buffer : m_command_buffers) {
		
		delete t_command_buffer;
	}
	if (m_command_pool) {
		
		delete m_command_pool;
	}
	for (auto* t_framebuffer : m_framebuffers) {
		
		delete t_framebuffer;
	}
	if (m_vertex_buffer) {
		
		delete m_vertex_buffer;
	}
	if (m_pipeline) {
		
		delete m_pipeline;
	}
	
	if (m_swapchain) {
		
		delete m_swapchain;
	}
	
	if (m_device) {
		
		delete m_device;
	}
	
	if (m_surface) {
		
		delete m_surface;
	}
	
	if (m_validation) {
		
		delete m_validation;
	}
	
	if (m_instance) {
		
		delete m_instance;
	}
}

void vk::context::render() {
	
	uint32_t image_index = m_swapchain->get_available_image_index(m_image_available_semaphore);
	
	std::vector<VkSemaphore> wait_semaphores {m_image_available_semaphore->get_semaphore()};
	std::vector<VkSemaphore> signal_semaphores {m_render_finished_semaphore->get_semaphore()};
	std::vector<VkPipelineStageFlags> stage_flags {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	std::vector<VkCommandBuffer> command_buffers {m_command_buffers[image_index]->get_command_buffer()};
	
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = nullptr;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores.data();
	submit_info.pWaitDstStageMask = stage_flags.data();
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = command_buffers.data();
	submit_info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size());
	submit_info.pSignalSemaphores = signal_semaphores.data();
	
	m_device->submit_graphical_queue(&submit_info);
	
	std::vector<VkSwapchainKHR> swapchains {m_swapchain->get_swapchain()};
	std::vector<uint32_t> image_indices;
	image_indices.push_back(image_index);
	m_device->queue_pressent(signal_semaphores, swapchains, image_indices);
}