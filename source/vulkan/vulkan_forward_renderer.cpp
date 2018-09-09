
// vulkan_forward_renderer.cpp
//
// source file for the RAII forward renderer
//
// author - Scott R Howell - https://github.com/thebombshell
// copyright - this document is free to use and transform, as long as authors and contributors are credited appropriately

#include "vulkan_forward_renderer.hpp"

#include "vulkan_context.hpp"
#include "vulkan_command_pool.hpp"
#include "vulkan_command_buffer.hpp"
#include "vulkan_descriptor_pool.hpp"
#include "vulkan_descriptor_set.hpp"
#include "vulkan_descriptor_set_layout.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"
#include "vulkan_mesh.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_pipeline_layout.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

#include "vector_math.hpp"

#include <limits>
#include <math.h>

//
// forward_renderer::draw_object
//

class vk::forward_renderer::draw_object {
	
	public:
	
	draw_object(vk::i_buffer& t_index_buffer, std::vector<vk::i_buffer*>& t_vertex_buffers, uint32_t t_element_count, VkIndexType t_index_type)
		: m_index_buffer{t_index_buffer}, m_vertex_buffers{t_vertex_buffers}, m_element_count{t_element_count}, m_index_type{t_index_type} {
		
	}
	~draw_object() {
		
	}
	
	void bind(vk::command_buffer& t_command_buffer) {
		
		t_command_buffer.bind_index_buffer(m_index_buffer, m_index_type);
		t_command_buffer.bind_vertex_buffers(m_vertex_buffers);
	}
	void draw(vk::command_buffer& t_command_buffer) {
		
		t_command_buffer.draw_indexed(m_element_count);
	}
	
	private:
	
	vk::i_buffer& m_index_buffer;
	std::vector<vk::i_buffer*> m_vertex_buffers;
	uint32_t m_element_count;
	VkIndexType m_index_type;
};

//
// forward_renderer::frame_object
//

vk::forward_renderer::frame_object::frame_object(vk::descriptor_pool& t_pool, vk::descriptor_set_layout& t_layout)
	: m_camera_uniform_buffer{nullptr} {
	
	m_descriptor_sets.push_back(new vk::descriptor_set(t_pool, t_layout));
	m_camera_uniform_buffer = new vk::buffer(t_pool.get_device(), sizeof(float) * 16 * 3, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
}

vk::forward_renderer::frame_object::~frame_object() {
	
	for (auto* t_descriptor_set : m_descriptor_sets) {
		
		delete t_descriptor_set;
	}
	delete m_camera_uniform_buffer;
}

std::vector<vk::descriptor_set*>& vk::forward_renderer::frame_object::get_descriptor_sets() {
	
	return m_descriptor_sets;
}

vk::buffer& vk::forward_renderer::frame_object::get_camera_uniform_buffer() {
	
	return *m_camera_uniform_buffer;
}

//
// forward_renderer
//

vk::forward_renderer::forward_renderer(vk::context& t_context) : i_device_object{t_context.get_device()}, m_context{t_context}
	, m_graphics_pool{nullptr}, m_default_vertex_shader{nullptr}, m_default_fragment_shader{nullptr}, m_render_pass{nullptr}
	, m_default_layout{nullptr}, m_default_pipeline{nullptr}
	, m_depth_images{ m_context.get_swapchain().get_image_views().size() }
	, m_depth_views{ m_context.get_swapchain().get_image_views().size() }
	, m_framebuffers{ m_context.get_swapchain().get_image_views().size() }
	, m_depthbuffers{ m_context.get_swapchain().get_image_views().size() }
	, m_frames{ VK_RENDERER_MAX_CONCURRENT_FRAMES } {
	
	uint32_t i{0};
	
	// graphics pool
	
	m_graphics_pool = new vk::command_pool(m_device, m_device.get_graphical_queue_family_index(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	
	// vertex shader
	
	m_default_vertex_shader = new vk::shader_module(m_device, VK_SHADER_STAGE_VERTEX_BIT, "basic_shader.vert.spv");
	
	// fragment shader
	
	m_default_fragment_shader = new vk::shader_module(m_device, VK_SHADER_STAGE_FRAGMENT_BIT, "basic_shader.frag.spv");
	
	// render pass
	
	VkFormat present_format = m_context.get_swapchain().get_surface_format().format;
	VkFormat depth_format = m_device.get_physical_device().find_supported_format(vk::get_depth_formats(), VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL);
	std::vector<VkAttachmentDescription> color_attachments
		{ { vk::get_present_attachment_description(present_format) }
		, { vk::get_depth_stencil_attachment_description(depth_format)} };
	std::vector<VkAttachmentReference> attachments{{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}, {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL}};
	VkSubpassDescription subpass = vk::get_graphics_subpass_description
		( 0, nullptr, 1, &attachments[0], nullptr, &attachments[1] );
	VkSubpassDependency subpass_dependancy = vk::get_external_subpass_dependancy
		( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		, 0
		, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
	m_render_pass = new vk::render_pass(m_device, &color_attachments[0], static_cast<uint32_t>(color_attachments.size()), &subpass, 1, &subpass_dependancy, 1);
	
	// descriptor layout
	
	std::vector<VkDescriptorSetLayoutBinding> default_descriptors {{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr}};
	m_default_descriptor_layouts.push_back(new vk::descriptor_set_layout(m_device, default_descriptors));
	
	// pipeline layout
	
	m_default_layout = new vk::pipeline_layout(m_device, m_default_descriptor_layouts);
	
	// descriptor pool
	
	std::vector<VkDescriptorPoolSize> sizes { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_RENDERER_MAX_CONCURRENT_FRAMES} };
	m_descriptor_pool = new vk::descriptor_pool(m_device, VK_RENDERER_MAX_CONCURRENT_FRAMES, sizes);
	
	// pipeline
	
	std::vector<vk::shader_module*> default_shader_modules {m_default_vertex_shader, m_default_fragment_shader};
	m_default_pipeline = new vk::graphics_pipeline(m_device, *m_render_pass, *m_default_layout, m_context.get_swapchain().get_extent()
		, present_format, default_shader_modules, vk::vertex::pos_col_nrm::get_vertex_definition());
	
	// frame objects
	
	std::vector<vk::image_view*> image_views{2};
	for (i = 0; i < VK_RENDERER_MAX_CONCURRENT_FRAMES; ++i) {
		
		m_frames[i] = new vk::render_frame(m_context.get_swapchain(), *m_graphics_pool);
		m_frame_objects.insert({m_frames[i], new vk::forward_renderer::frame_object(*m_descriptor_pool, *m_default_descriptor_layouts[0])});
	}
	m_frame_iterator = m_frames.begin();
	
	// frame buffers
	
	VkExtent2D extent2d = m_context.get_swapchain().get_extent();
	VkExtent3D extent3d = { extent2d.width, extent2d.height, 1 };
	for (i = 0; i < m_context.get_swapchain().get_image_views().size(); ++i) {
		
		image_views[0] = m_context.get_swapchain().get_image_views()[i];
		m_depth_images[i] = new vk::depth_image(m_device, depth_format, extent3d);
		image_views[1] = m_depth_views[i] = new vk::image_view(m_device, *m_depth_images[i], depth_format, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
		m_framebuffers[i] = new vk::framebuffer(*m_render_pass, image_views, extent2d, 1);
	}
}

vk::forward_renderer::~forward_renderer() {
	
	for (auto& t_entry : m_frame_objects) {
		
		delete t_entry.second;
	}
	
	delete m_default_pipeline;
	delete m_default_layout;
	delete m_descriptor_pool;
	
	for (auto* t_descriptor_layout : m_default_descriptor_layouts) {
		
		delete t_descriptor_layout;
	}
	
	delete m_render_pass;
	delete m_default_vertex_shader;
	delete m_default_fragment_shader;
	
	for (auto* t_draw : m_draws) {
		
		delete t_draw;
	}
	
	for (auto* t_frames : m_frames) {
		
		delete t_frames;
	}
	
	for (auto* t_framebuffer : m_framebuffers) {
		
		delete t_framebuffer;
	}
	
	for (auto* t_depthbuffer : m_depthbuffers) {
		
		delete t_depthbuffer;
	}
	
	for (auto* t_depth_view : m_depth_views) {
		
		delete t_depth_view;
	}
	
	delete m_graphics_pool;
}

const vk::forward_renderer::draw_object* vk::forward_renderer::create_draw
	( vk::i_buffer& t_index_buffer, std::vector<vk::i_buffer*>& t_vertex_buffers
	, uint32_t t_element_count, VkIndexType t_index_types) {
	
	vk::forward_renderer::draw_object* output = 
		new vk::forward_renderer::draw_object(t_index_buffer, t_vertex_buffers, t_element_count, t_index_types);
	m_draws.push_back(output);
	return output;
}

void vk::forward_renderer::render() {
	
	uint32_t image_index {0};
	if (m_frame_iterator == m_frames.end()) {
		
		m_frame_iterator = m_frames.begin();
	}
	image_index = (*m_frame_iterator)->get_image_index();
	vk::command_buffer& command_buffer {(*m_frame_iterator)->get_command_buffer()};
	vk::forward_renderer::frame_object* frame_object = m_frame_objects[*m_frame_iterator];
	
	float camera_matrices[16 * 3];
	
	float eye[3] { 5.0f, 5.0f, 0.0f };
	float look[3] { 0.0f, 0.0f, 0.0f };
	float up[3] { 0.0f, 0.0f, 1.0f };
	mat4_identity(&camera_matrices[0]);
	mat4_lookat(&camera_matrices[16], &eye[0], &look[0], &up[0]);
	mat4_perspective(&camera_matrices[32], 1280.0f / 720.0f, 3.14f * 0.25f, 0.01f, 100.0f);
	frame_object->get_camera_uniform_buffer().map(&camera_matrices[0], sizeof(float) * 16 * 3);
	
	VkDescriptorBufferInfo buffer_info { frame_object->get_camera_uniform_buffer().get_buffer(), 0, sizeof(float) * 16 * 3 };
	frame_object->get_descriptor_sets()[0]->write(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, 1, nullptr, &buffer_info, nullptr);
	
	std::vector<VkClearValue> clear_values{2};
	clear_values[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
	clear_values[1].depthStencil = {1.0f, 0};
	
	command_buffer.reset();
	command_buffer.begin();
	command_buffer.begin_render_pass(m_default_pipeline->get_render_pass(), *m_framebuffers[image_index]
		, {{0, 0}, m_context.get_swapchain().get_extent()}, &clear_values[0], 2);
	command_buffer.bind_pipeline(*m_default_pipeline);
	command_buffer.bind_descriptor_sets(frame_object->get_descriptor_sets(), *m_default_layout);
	
	for (auto* t_draw : m_draws) {
		
		t_draw->bind(command_buffer);
		t_draw->draw(command_buffer);
	}
	
	command_buffer.end_render_pass();
	command_buffer.end();
	
	(*m_frame_iterator)->submit();
	
	++m_frame_iterator;
}

vk::context& vk::forward_renderer::get_context() {
	
	return m_context;
}

const vk::context& vk::forward_renderer::get_context() const {
	
	return m_context;
}

vk::render_pass& vk::forward_renderer::get_render_pass() {
	
	return *m_render_pass;
}

const vk::render_pass& vk::forward_renderer::get_render_pass() const {
	
	return *m_render_pass;
}