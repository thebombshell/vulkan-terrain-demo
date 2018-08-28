
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
#include "vulkan_framebuffer.hpp"
#include "vulkan_mesh.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

#include <limits>

//
// forward_renderer::draw
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
// forward_renderer
//

vk::forward_renderer::forward_renderer(vk::context& t_context) : i_device_object{t_context.get_device()}, m_context{t_context}
	, m_graphics_pool{nullptr}, m_default_vertex_shader{nullptr}, m_default_fragment_shader{nullptr}, m_render_pass{nullptr}, m_default_pipeline{nullptr}, m_frames{VK_RENDERER_MAX_CONCURRENT_FRAMES} {
	
	uint32_t i{0};
	
	m_graphics_pool = new vk::command_pool(m_device, m_device.get_graphical_queue_family_index(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	m_default_vertex_shader = new vk::shader_module(m_device, VK_SHADER_STAGE_VERTEX_BIT, "basic_shader.vert.spv");
	m_default_fragment_shader = new vk::shader_module(m_device, VK_SHADER_STAGE_FRAGMENT_BIT, "basic_shader.frag.spv");
	std::vector<vk::shader_module*> default_shader_modules {m_default_vertex_shader, m_default_fragment_shader};
	
	VkAttachmentDescription color_attachment = vk::get_present_attachment_description(m_context.get_swapchain().get_surface_format().format);
	std::vector<VkAttachmentReference> attachments = {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};
	VkSubpassDescription subpass = vk::get_graphics_subpass_description( nullptr, &attachments );
	VkSubpassDependency subpass_dependancy = vk::get_external_subpass_dependancy
		( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		, 0
		, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
	m_render_pass = new vk::render_pass(m_device, &color_attachment, 1, &subpass, 1, &subpass_dependancy, 1);
	m_default_pipeline = new vk::graphics_pipeline(m_device, *m_render_pass, m_context.get_swapchain().get_extent(), m_context.get_swapchain().get_surface_format().format
		, default_shader_modules, vk::vertex::pos_col_nrm::get_vertex_definition());
	std::vector<vk::image_view*> image_views{1};
	
	for (; i < VK_RENDERER_MAX_CONCURRENT_FRAMES; ++i) {
		
		m_frames[i] = new vk::render_frame(m_context.get_swapchain(), *m_graphics_pool);
	}
	m_frame_iterator = m_frames.begin();
	
	for (i = 0; i < m_context.get_swapchain().get_image_views().size(); ++i) {
		
		image_views[0] = m_context.get_swapchain().get_image_views()[i];
		m_framebuffers.push_back(new vk::framebuffer(*m_render_pass, image_views, m_context.get_swapchain().get_extent(), 1));
	}
}

vk::forward_renderer::~forward_renderer() {
	
	delete m_default_pipeline;
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
	
	VkClearValue clear_value {1.0f, 0.0f, 0.0f, 1.0f};
	
	command_buffer.reset();
	command_buffer.begin();
	command_buffer.begin_render_pass(m_default_pipeline->get_render_pass(), *m_framebuffers[image_index]
			, {{0, 0}, m_context.get_swapchain().get_extent()}, &clear_value, 1);
	command_buffer.bind_pipeline(*m_default_pipeline);
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