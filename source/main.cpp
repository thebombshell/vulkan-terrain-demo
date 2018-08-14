
// /main.cpp

#include <exception>
#include <iostream>
#include <mingw.thread.h>
#include "window.hpp"
#include "vulkan_context.hpp"
#include "vulkan_device.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_command_buffer.hpp"

using namespace vk_terrain_demo;

window* app_window;
vk::context* vulkan;
vk::command_pool* pool;
vk::command_buffer* main_command;
bool is_running = true;
bool should_wait = false;

void render_init() {
	
	vulkan = new vk::context{app_window->get_handle(), GetModuleHandle(0)};
	pool = new vk::command_pool(vulkan->get_device(), vulkan->get_device().get_graphical_queue_family_index());
	main_command = new vk::command_buffer(*pool);
}

void render_shutdown() {
	
	delete main_command;
	delete pool;
	delete vulkan;
}

void render() {
	
	
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