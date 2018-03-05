
// /main.cpp

#include <exception>
#include <iostream>
#include <mingw.thread.h>
#include "window.hpp"
#include "vulkan_context.hpp"

using namespace vk_terrain_demo;

window* app_window;
vk::context* vulkan;
bool is_running = true;
bool should_wait = false;
void render_loop() {
	
	try {
		
		vulkan = new vk::context{app_window->get_handle(), GetModuleHandle(0)};
		while (is_running) {
			
		}
		delete vulkan;
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