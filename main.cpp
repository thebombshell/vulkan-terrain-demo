
// /main.cpp

#include <exception>
#include <iostream>
#include <mingw.thread.h>
#include "window.hpp"
#include "vulkan_context.hpp"

using namespace vk_terrain_demo;

vk::context* vulkan;
bool is_running = true;
void render_loop() {
	
	while (is_running) {
		
		vulkan->render();
	}
}

int main() {
	
	try {
		
		window app_window{"vulkan terrain demo", 1280, 720};
		vulkan = new vk::context{app_window.get_handle(), GetModuleHandle(0)};
		std::thread render_thread{render_loop};
		app_window.run();
		is_running = false;
		render_thread.join();
	}
	catch (const std::exception& t_exception) {
		
		std::cout << t_exception.what() << "\n";
		std::cout << "press enter to exit";
		
		std::cin.get();
	}
	
	return 0;
}