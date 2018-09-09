
// /main.cpp

#include <exception>
#include <iostream>
#include <math.h>
#include <mingw.thread.h>
#include "window.hpp"

#include "vulkan_buffer.hpp"
#include "vulkan_context.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_fence.hpp"
#include "vulkan_forward_renderer.hpp"
#include "vulkan_mesh.hpp"

using namespace vk_terrain_demo;

window* app_window;
vk::context* vulkan;
vk::forward_renderer* renderer;

vk::staging_buffer* index_staging_buffer;
vk::staging_buffer* vertex_staging_buffer;
vk::staged_index_buffer* index_buffer;
vk::staged_vertex_buffer* vertex_buffer;
std::vector<vk::i_buffer*> vertex_buffers;

vk::command_pool* command_pool;
vk::command_buffer* command_buffer;

bool is_running = true;
bool should_wait = false;

void render_init() {
	
	vulkan = new vk::context{app_window->get_handle(), GetModuleHandle(0)};
	renderer = new vk::forward_renderer(*vulkan);
	
	index_staging_buffer = new vk::staging_buffer(vulkan->get_device(), sizeof(uint16_t) * 36);
	vertex_staging_buffer = new vk::staging_buffer(vulkan->get_device(), sizeof(float) * 10 * 8);
	
	float unit = 1.0f / sqrt(1.0f + 1.0f + 1.0f);
	std::vector<uint16_t> index_data 
		{ 0, 4, 1, 4, 5, 1
		, 1, 5, 2, 5, 6, 2
		, 2, 6, 3, 6, 7, 3
		, 3, 7, 0, 7, 4, 0
		, 0, 1, 3, 1, 2, 3
		, 5, 6, 4, 6, 7, 4
		};
	std::vector<float> vertex_data
		{ -0.5f, -0.5f, -0.5f	, 0.0f, 0.0f, 0.0f, 1.0f, -unit, -unit, -unit
		, 0.5f, -0.5f, -0.5f	, 1.0f, 0.0f, 0.0f, 1.0f, unit, -unit, -unit
		, 0.5f, 0.5f, -0.5f		, 1.0f, 1.0f, 0.0f, 1.0f, unit, unit, -unit
		, -0.5f, 0.5f, -0.5f	, 0.0f, 1.0f, 0.0f, 1.0f, -unit, unit, -unit
		, -0.5f, -0.5f, 0.5f	, 0.0f, 0.0f, 1.0f, 1.0f, -unit, -unit, unit
		, 0.5f, -0.5f, 0.5f		, 1.0f, 0.0f, 1.0f, 1.0f, unit, -unit, unit
		, 0.5f, 0.5f, 0.5f		, 1.0f, 1.0f, 1.0f, 1.0f, unit, unit, unit
		, -0.5f, 0.5f, 0.5f		, 0.0f, 1.0f, 1.0f, 1.0f, -unit, unit, unit
		};
	index_staging_buffer->map(&index_data[0], sizeof(uint16_t) * 36);
	vertex_staging_buffer->map(&vertex_data[0], sizeof(vk::vertex::pos_col_nrm) * 8);
	
	index_buffer = new vk::staged_index_buffer(vulkan->get_device(), *index_staging_buffer, sizeof(uint16_t) * 36);
	vertex_buffer = new vk::staged_vertex_buffer(vulkan->get_device(), *vertex_staging_buffer, sizeof(float) * 10 * 8);
	
	vk::command_pool command_pool{vulkan->get_device(), static_cast<uint32_t>(vulkan->get_device().get_graphical_queue_family_index())};
	vk::command_buffer command_buffer{command_pool};
	vk::fence fence{vulkan->get_device(), 0};
	std::vector<VkSubmitInfo> submit_infos;
	VkCommandBuffer buffer = command_buffer.get_command_buffer();
	
	command_buffer.begin();
	command_buffer.copy_staged_buffer(*index_buffer, sizeof(uint16_t) * 36);
	command_buffer.copy_staged_buffer(*vertex_buffer, sizeof(float) * 10 * 8);
	command_buffer.end();
	
	submit_infos.push_back(VkSubmitInfo
		{ VK_STRUCTURE_TYPE_SUBMIT_INFO
		, 0
		, 0
		, nullptr
		, nullptr
		, 1
		, &buffer
		, 0
		, nullptr
		});
	
	vulkan->get_device().submit_graphical_queue(submit_infos, &fence);
	
	fence.wait_and_reset();
	
	vertex_buffers.push_back(vertex_buffer);
	renderer->create_draw(*index_buffer, vertex_buffers, 36, VK_INDEX_TYPE_UINT16);
}

void render_shutdown() {
	
	delete index_buffer;
	delete vertex_buffer;
	delete index_staging_buffer;
	delete vertex_staging_buffer;
	
	delete renderer;
	delete vulkan;
}

void render() {
	
	renderer->render();
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