MINGW = C:/Program Files/mingw-w64/x86_64-7.2.0-win32-seh-rt_v5-rev1/mingw64
VULKAN = C:/VulkanSDK/1.0.65.1

CXX = $(MINGW)/bin/g++
GDB = $(MINGW)/bin/gdb
SPIRV = $(VULKAN)/bin/glslangValidator

CFLAGS = -g -std=c++11 -static-libgcc -static-libstdc++ -Wall -Werror -DVK_USE_PLATFORM_WIN32_KHR
GDBFLAGS = 
SPIRVFLAGS = 

SOURCE = *.cpp
OBJS = $(patsubst %.cpp, %.o, $(wildcard $(SOURCE)))
INC = -I$(VULKAN)/Include

LDDIR = -L$(VULKAN)/Lib
LDLIBS = -lvulkan-1 -lgdi32
LDFLAGS = $(LDDIR) $(LDLIBS)

all: vk_terrain_demo shaders

vk_terrain_demo: $(OBJS)
	$(CXX) $(CFLAGS) $(INC) $(OBJS) -o vk_terrain_demo $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c $< -o $@

shaders: vert.spv frag.spv

vert.spv: basic_shader.vert
	$(SPIRV) $(SPIRVFLAGS) -V basic_shader.vert

frag.spv: basic_shader.frag
	$(SPIRV) $(SPIRVFLAGS) -V basic_shader.frag

clean:
	rm *.o vk_terrain_demo -f

debug:
	$(GDB) $(GDBFLAGS) vk_terrain_demo.exe