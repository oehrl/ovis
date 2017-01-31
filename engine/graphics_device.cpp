#include "graphics_device.hpp"
#include "log.hpp"
#include "range.hpp"
#include "uniform_buffer.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "vertex_source.hpp"
#include "index_buffer.hpp"

GraphicsDevice::GraphicsDevice(SDL_Window* window) :
    m_context(nullptr),
    m_bound_array_buffer(0),
    m_bound_element_array_buffer(0),
    m_bound_program(0),
    m_active_texture_unit(0),
    m_alpha_blending_enabled(false),
    m_depth_test_enabled(false),
    m_depth_writes_enabled(true)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    
#ifndef __IPHONEOS__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    
    m_context = SDL_GL_CreateContext(window);
    SDL_assert(m_context != nullptr);
    SDL_GL_MakeCurrent(window, m_context);
    
    LogI("OpenGL version: ", glGetString(GL_VERSION));
    
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.max_vertex_attribs);
    SDL_assert(m_caps.max_vertex_attribs >= 8);
    m_vertex_attrib_array_states.resize(m_caps.max_vertex_attribs, false);

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_caps.num_texture_units);
    SDL_assert(m_caps.num_texture_units >= 8);
    m_bound_textures.resize(m_caps.num_texture_units, 0);
    

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_caps.num_vertex_texture_units);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
#ifndef __IPHONEOS__
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
#endif
}

GraphicsDevice::~GraphicsDevice()
{
    SDL_assert(m_graphics_resources.size() == 0);
}

void GraphicsDevice::Draw
(
    const DrawItem& draw_item
)
{
    SDL_assert(draw_item.shader_program != nullptr);
    SDL_assert(draw_item.vertex_source != nullptr);
    
    if (draw_item.alpha_blending_enabled)
    {
        EnableAlphaBlending();
    } else
    {
        DisableAlphaBlending();
    }
    
    switch (draw_item.depth_buffer_state)
    {
        case DepthBufferState::ENABLED:
            EnableDepthTest();
            EnableDepthWrites();
            break;
            
        case DepthBufferState::READONLY:
            EnableDepthTest();
            DisableDepthWrites();
            break;
            
        case DepthBufferState::DISABLED:
            DisableDepthTest();
            break;
    }

    draw_item.shader_program->Bind();
    draw_item.vertex_source->Bind();

    if (draw_item.index_buffer == nullptr)
    {
#ifdef DEBUG
        glValidateProgram(draw_item.shader_program->m_program_name);
        GLint validation_status = 0;
        glGetProgramiv(draw_item.shader_program->m_program_name, GL_VALIDATE_STATUS, &validation_status);
        SDL_assert(validation_status == GL_TRUE);
#endif

        glDrawArrays(
            static_cast<GLenum>(draw_item.primitive_topology),
            draw_item.start,
            draw_item.count
        );
    } else
    {
        draw_item.index_buffer->Bind();
        
#ifdef DEBUG
        glValidateProgram(draw_item.shader_program->m_program_name);
        GLint validation_status = 0;
        glGetProgramiv(draw_item.shader_program->m_program_name, GL_VALIDATE_STATUS, &validation_status);
        SDL_assert(validation_status == GL_TRUE);
#endif

        glDrawElements(
            static_cast<GLenum>(draw_item.primitive_topology),
            draw_item.count,
            static_cast<GLenum>(draw_item.index_buffer->description().index_format),
            reinterpret_cast<GLvoid*>(draw_item.start * draw_item.index_buffer->bytes_per_index())
        );
    }
}

void GraphicsDevice::Clear()
{
    EnableDepthWrites();
    glClearColor(0.0f, 52.0f / 255.0f, 138.0f / 255.0f, 1.0f);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
