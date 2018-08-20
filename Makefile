MINGW = C:/Program Files/mingw-w64/x86_64-7.2.0-win32-seh-rt_v5-rev1/mingw64
VULKAN = C:/VulkanSDK/1.0.68.0
SOURCE_DIR = source
SHADER_DIR = shaders
OBJECT_DIR = objects
BUILD_DIR = build

CXX = $(MINGW)/bin/g++
GDB = $(MINGW)/bin/gdb
SPIRV = $(VULKAN)/bin/glslangValidator

CFLAGS = -g -Og -std=c++11 -static-libgcc -static-libstdc++ -Wall -Werror -Wfatal-errors -DVK_USE_PLATFORM_WIN32_KHR
GDBFLAGS = -cd $(BUILD_DIR)
SPIRVFLAGS = 

MAIN_SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
MAIN_OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(MAIN_SOURCES))
VULKAN_SOURCES := $(wildcard $(SOURCE_DIR)/vulkan/*.cpp)
VULKAN_OBJECTS := $(patsubst $(SOURCE_DIR)/vulkan/%.cpp, $(OBJECT_DIR)/%.o, $(VULKAN_SOURCES))
SHADER_SOURCES := $(wildcard $(SHADER_DIR)/*)
SHADER_OBJECTS := $(patsubst $(SHADER_DIR)/%, $(BUILD_DIR)/%.spv, $(SHADER_SOURCES))

INC = -I$(VULKAN)/Include -Isource -Isource/vulkan

LDDIR = -L$(VULKAN)/Lib
LDLIBS = -lvulkan-1 -lgdi32
LDFLAGS = $(LDDIR) $(LDLIBS)

all: vk_terrain_demo shaders

vk_terrain_demo: main vulkan
	$(CXX) $(CFLAGS) $(INC) $(MAIN_OBJECTS) $(VULKAN_OBJECTS) -o $(BUILD_DIR)/vk_terrain_demo $(LDFLAGS)

main: $(MAIN_OBJECTS)

vulkan: $(VULKAN_OBJECTS)

shaders: $(SHADER_DIRECTORIES) $(SHADER_OBJECTS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.hpp
	$(CXX) $(CFLAGS) $(INC) -c $< -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/vulkan/%.cpp $(SOURCE_DIR)/vulkan/%.hpp
	$(CXX) $(CFLAGS) $(INC) -c $< -o $@

$(BUILD_DIR)/%.spv: $(SHADER_DIR)/%
	$(SPIRV) $(SPIRVFLAGS) -V $< -o $@

vert.spv: basic_shader.vert
	$(SPIRV) $(SPIRVFLAGS) -V basic_shader.vert

frag.spv: basic_shader.frag
	$(SPIRV) $(SPIRVFLAGS) -V basic_shader.frag

clean: rm del

rm:
	-rm $(BUILD_DIR)/* $(OBJECT_DIR)/* -f

del:
	-del $(BUILD_DIR)\\* $(OBJECT_DIR)\\* /F /Q

debug:
	$(GDB) $(GDBFLAGS) vk_terrain_demo.exe