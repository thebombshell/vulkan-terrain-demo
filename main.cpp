
// /main.cpp

#include <exception>
#include <iostream>
#include "window.hpp"
#include "vulkan_context.hpp"

using namespace vk_terrain_demo;

int main() {
	
	try {
		
		window app_window{"vulkan terrain demo", 1280, 720};
		vk::context vulkan {app_window.get_handle(), GetModuleHandle(0)};
		
		app_window.run();
	}
	catch (const std::exception& t_exception) {
		
		std::cout << t_exception.what() << "\n";
		std::cout << "press enter to exit";
		
		std::cin.get();
	}
	
	return 0;
}