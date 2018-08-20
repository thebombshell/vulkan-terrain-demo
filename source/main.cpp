
// /main.cpp

#include <exception>
#include <iostream>
#include <mingw.thread.h>
#include "window.hpp"

#include "vulkan_context.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_mesh.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

using namespace vk_terrain_demo;

const uint32_t MAX_CONCURRENT_FRAMES {2};

window* app_window;
vk::context* vulkan;
vk::command_pool* graphics_pool;
vk::command_pool* present_pool;
std::vector<vk::command_buffer*> graphics_buffers;
vk::shader_module* default_vertex_shader;
vk::shader_module* default_fragment_shader;
vk::graphics_pipeline* default_pipeline;
std::vector<vk::framebuffer*> framebuffers;
std::vector<vk::semaphore*> image_available_semaphores;
std::vector<vk::semaphore*> render_complete_semaphores;
std::vector<vk::fence*> frame_ready_fences;
uint32_t frame_index {0};

bool is_running = true;
bool should_wait = false;

struct vertex {
	
	public:
	
	vertex() : position{0.0f, 0.0f, 0.0f}, color{1.0f, 1.0f, 1.0f, 1.0f}, normal{0.0f, 0.0f, 1.0f} {
	}
	
	static const vk::vertex_definition& get_definition() {
		
		static vk::vertex_definition* definition{nullptr};
		
		if (definition == nullptr) {
			
			std::vector<vk::vertex_attribute> attributes
				{ {vk::vertex_purpose::VPOS, offsetof(vertex, position)}
				, {vk::vertex_purpose::VCOL, offsetof(vertex, color)}
				, {vk::vertex_purpose::VNRM, offsetof(vertex, normal)}
				};
			std::vector<vk::vertex_binding> bindings{{sizeof(vertex), attributes}};
			definition = new vk::vertex_definition(bindings);
		}
		
		return *definition;
	}
	
	float position[3];
	float color[4];
	float normal[3];
};

void render_init() {
	
	uint32_t i{0};
	vulkan = new vk::context{app_window->get_handle(), GetModuleHandle(0)};
	vk::device& device{vulkan->get_device()};
	
	graphics_pool = new vk::command_pool(device, device.get_graphical_queue_family_index());
	present_pool = new vk::command_pool(device, device.get_present_queue_family_index());
	default_vertex_shader = new vk::shader_module(device, VK_SHADER_STAGE_VERTEX_BIT, "basic_shader.vert.spv");
	default_fragment_shader = new vk::shader_module(device, VK_SHADER_STAGE_FRAGMENT_BIT, "basic_shader.frag.spv");
	std::vector<vk::shader_module*> default_shader_modules {default_vertex_shader, default_fragment_shader};
	default_pipeline = new vk::graphics_pipeline(device, vulkan->get_swapchain().get_extent(), vulkan->get_swapchain().get_surface_format().format
		, default_shader_modules, vertex::get_definition());
	std::vector<vk::image_view*> image_views{1};
	
	for (; i < MAX_CONCURRENT_FRAMES; ++i) {
		
		image_available_semaphores.push_back(new vk::semaphore(device));
		render_complete_semaphores.push_back(new vk::semaphore(device));
		frame_ready_fences.push_back(new vk::fence(device));
	}
	
	for (i = 0; i < vulkan->get_swapchain().get_image_views().size(); ++i) {
		
		image_views[0] = vulkan->get_swapchain().get_image_views()[i];
		framebuffers.push_back(new vk::framebuffer(default_pipeline->get_render_pass(), image_views, vulkan->get_swapchain().get_extent(), 1));
		graphics_buffers.push_back(new vk::command_buffer(*graphics_pool));
		
		VkClearValue clear_value {1.0f, 0.0f, 0.0f, 1.0f};
		
		graphics_buffers[i]->begin();
		graphics_buffers[i]->begin_render_pass(default_pipeline->get_render_pass(), *framebuffers[framebuffers.size() - 1]
				, {{0, 0}, vulkan->get_swapchain().get_extent()}, &clear_value, 1);
		graphics_buffers[i]->end_render_pass();
		graphics_buffers[i]->end();
	}
}

void render_shutdown() {
	
	for (auto* t_semaphore : image_available_semaphores) {
		
		delete t_semaphore;
	}
	
	for (auto* t_semaphore : render_complete_semaphores) {
		
		delete t_semaphore;
	}
	
	for (auto* t_fence : frame_ready_fences) {
		
		delete t_fence;
	}
	
	for (auto* t_framebuffer : framebuffers) {
		
		delete t_framebuffer;
	}
	
	graphics_buffers.clear();
	delete graphics_pool;
	delete present_pool;
	delete vulkan;
}

void render() {
	
	frame_index = (frame_index + 1) % MAX_CONCURRENT_FRAMES;
	frame_ready_fences[frame_index]->wait_and_reset();
	
	uint32_t image_index = vulkan->get_swapchain().get_available_image_index(image_available_semaphores[frame_index]);
	
	VkPipelineStageFlags stages[1] {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	std::vector<VkSemaphore> image_semaphores { 1 };
	std::vector<VkSemaphore> render_semaphores { 1 };
	std::vector<VkSubmitInfo> graphics_submit_infos { 1 };
	std::vector<VkSwapchainKHR> swapchains { 1 };
	std::vector<uint32_t> image_indices { 1 };
	
	image_indices[0] = image_index;
	
	image_semaphores[0] = image_available_semaphores[frame_index]->get_semaphore();
	render_semaphores[0] = render_complete_semaphores[frame_index]->get_semaphore();
	swapchains[0] = vulkan->get_swapchain().get_swapchain();
	
	VkCommandBuffer buffer = graphics_buffers[image_index]->get_command_buffer();
	
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
	
	vulkan->get_device().submit_graphical_queue(graphics_submit_infos, frame_ready_fences[frame_index]);
	vulkan->get_device().queue_present(render_semaphores, swapchains, image_indices);
}

void render_loop() {
	
	try {
		
		render_init();
		while (is_running) {
			
			render();
		}
		render_shutdown();
	}
	catch (const std::exception& t_exception) {
		
		std::cout << t_exception.what() << "\n";
		std::cout << "press enter to exit";
		is_running = false;
		should_wait = true;
	}
}

int main() {
	
	try {
		
		app_window = new window("vulkan terrain demo", 1280, 720);
		std::thread render_thread{render_loop};
		app_window->run();
		is_running = false;
		render_thread.join();
		delete app_window;
	}
	catch (const std::exception& t_exception) {
		
		std::cout << t_exception.what() << "\n";
		std::cout << "press enter to exit";
		is_running = false;
		should_wait = true;
	}
	
	if (should_wait) {
		
		std::cin.get();
	}
	
	return 0;
}