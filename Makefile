MINGW = C:/Program Files/mingw-w64/x86_64-7.2.0-win32-seh-rt_v5-rev1/mingw64
VULKAN = C:/VulkanSDK/1.0.65.1
CC = $(MINGW)/bin/gcc
CXX = $(MINGW)/bin/g++
GDB = $(MINGW)/bin/gdb
SPIRV = $(VULKAN)/bin/glslangValidator
CFLAGS = -g -std=c++11 -static-libgcc -static-libstdc++ -Wall -Werror -DVK_USE_PLATFORM_WIN32_KHR
VULKAN_SOURCE = vulkan_context.cpp vulkan_instance.cpp vulkan_surface.cpp vulkan_device.cpp vulkan_swapchain.cpp vulkan_graphics_pipeline.cpp
VULKAN_HEADER = vulkan.hpp vulkan_context.hpp vulkan_instance.hpp vulkan_surface.hpp vulkan_device.hpp vulkan_swapchain.hpp vulkan_graphics_pipeline.hpp
VULKAN_OBJS = vulkan_context.o vulkan_instance.o vulkan_surface.o vulkan_device.o vulkan_swapchain.o vulkan_graphics_pipeline.cpp
OBJS = main.o window.o $(VULKAN_OBJS)
INC = -I$(VULKAN)/Include
LDDIR = -L$(VULKAN)/Lib
LDLIBS = -lvulkan-1 -lgdi32
LDFLAGS = $(LDDIR) $(LDLIBS)

all: vk_terrain_demo shaders

vk_terrain_demo: main.o
	$(CXX) $(CFLAGS) $(INC) $(OBJS) -o vk_terrain_demo $(LDFLAGS)

main.o: vulkan window.o main.cpp
	$(CXX) $(CFLAGS) $(INC) -c main.cpp

vulkan: $(VULKAN_SOURCE) $(VULKAN_HEADER)
	$(CXX) $(CFLAGS) $(INC) -c $(VULKAN_SOURCE)

window.o: window.hpp window.cpp
	$(CXX) $(CFLAGS) $(INC) -c window.cpp

shaders: vert.spv frag.spv

vert.spv: basic_shader.vert
	$(SPIRV) -V basic_shader.vert

frag.spv: basic_shader.frag
	$(SPIRV) -V basic_shader.frag

clean:
	rm *o vk_terrain_demo -f

debug:
	$(GDB) vk_terrain_demo.exe