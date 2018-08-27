
#include "vulkan_command_buffer.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_swapchain.hpp"

#include <limits>

//
// render_frame
//

vk::render_frame::render_frame(vk::swapchain& t_swapchain, vk::command_pool& t_pool) 
	: i_device_object{t_swapchain}, m_swapchain{t_swapchain}
	, m_graphics_buffer{nullptr}, m_image_available_semaphore{nullptr}, m_render_complete_semaphore{nullptr}, m_ready_fence{nullptr}
	, m_image_index{std::numeric_limits<uint32_t>::max()} {
	
	m_graphics_buffer = new vk::command_buffer(t_pool);
	m_image_available_semaphore = new vk::semaphore(m_device);
	m_render_complete_semaphore = new vk::semaphore(m_device);
	m_ready_fence = new vk::fence(m_device, VK_FENCE_CREATE_SIGNALED_BIT);
	
}

vk::render_frame::~render_frame() {
	
	delete m_graphics_buffer;
	delete m_image_available_semaphore;
	delete m_render_complete_semaphore;
	delete m_ready_fence;
}

uint32_t vk::render_frame::get_image_index() {
	
	if (m_image_index != std::numeric_limits<uint32_t>::max()) {
		
		throw std::runtime_error("Can not request swapchain image on frame which is already in use");
	}
	
	m_ready_fence->wait_and_reset();
	return m_image_index = m_swapchain.get_available_image_index(m_image_available_semaphore);
}

void vk::render_frame::submit() {
	
	VkPipelineStageFlags stages[1] {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	std::vector<VkSemaphore> image_semaphores { 1 };
	std::vector<VkSemaphore> render_semaphores { 1 };
	std::vector<VkSubmitInfo> graphics_submit_infos { 1 };
	std::vector<VkSwapchainKHR> swapchains { 1 };
	std::vector<uint32_t> image_indices { 1 };
	image_indices[0] = m_image_index;
	VkCommandBuffer buffer = m_graphics_buffer->get_command_buffer();
	
	image_semaphores[0] = m_image_available_semaphore->get_semaphore();
	render_semaphores[0] = m_render_complete_semaphore->get_semaphore();
	swapchains[0] = m_swapchain.get_swapchain();
	
	graphics_submit_infos[0] =
		{ VK_STRUCTURE_TYPE_SUBMIT_INFO
		, 0
		, 1
		, &image_semaphores[0]
		, &stages[0]
		, 1
		, &buffer
		, 1
		, &render_semaphores[0]
		};
	
	m_device.submit_graphical_queue(graphics_submit_infos, m_ready_fence);
	m_device.queue_present(render_semaphores, swapchains, image_indices);
	
	m_image_index = std::numeric_limits<uint32_t>::max();
}

vk::command_buffer& vk::render_frame::get_command_buffer() {
	
	return *m_graphics_buffer;
}

const vk::command_buffer& vk::render_frame::get_command_buffer() const {
	
	return *m_graphics_buffer;
}