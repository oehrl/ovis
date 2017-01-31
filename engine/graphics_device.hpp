#pragma once

#include <set>
#include <vector>

#include <SDL.h>

#include "application.hpp"
#include "gl.hpp"
#include "class.hpp"

class GraphicsResource;
class IndexBuffer;
class ShaderProgram;
class UniformBuffer;
class VertexBuffer;
class VertexSource;
class Texture;

enum class PrimitiveTopology
{
    POINTS = GL_POINTS,
    LINE_LIST = GL_LINES,
    LINE_STRIP = GL_LINE_STRIP,
    TRIANGLE_LIST = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
};

enum class DepthBufferState
{
    ENABLED,
    READONLY,
    DISABLED
};

struct DrawItem
{
    ShaderProgram* shader_program;
    VertexSource* vertex_source;
    IndexBuffer* index_buffer;
    PrimitiveTopology primitive_topology;
    Uint32 start;
    Uint32 count;
    DepthBufferState depth_buffer_state;
    bool alpha_blending_enabled;
};

class GraphicsDevice final
{
    friend class GraphicsResource;
    friend class IndexBuffer;
    friend class ShaderProgram;
    friend class Texture2D;
    friend class UniformBuffer;
    friend class VertexBuffer;
    friend class VertexSource;
    
    MAKE_NON_COPY_OR_MOVABLE(GraphicsDevice);
    
public:
    GraphicsDevice(SDL_Window* window);
    ~GraphicsDevice();

    void Draw(const DrawItem& draw_item);
    
    void Clear();

private:
    SDL_GLContext m_context;
    std::set<GraphicsResource*> m_graphics_resources;
    
    struct
    {
        GLint max_vertex_attribs;
        GLint num_texture_units;
        GLint num_vertex_texture_units;
    } m_caps;
    
    GLuint m_bound_array_buffer;
    GLuint m_bound_element_array_buffer;
    GLuint m_bound_program;
    GLuint m_active_texture_unit;
    bool m_alpha_blending_enabled;
    bool m_depth_test_enabled;
    bool m_depth_writes_enabled;
    std::vector<bool> m_vertex_attrib_array_states;
    std::vector<GLuint> m_bound_textures;
    
    inline void EnableDepthTest()
    {
        if (!m_depth_test_enabled)
        {
            glEnable(GL_DEPTH_TEST);
            m_depth_test_enabled = true;
        }
    }
    
    inline void DisableDepthTest()
    {
        if (m_depth_test_enabled)
        {
            glDisable(GL_DEPTH_TEST);
            m_depth_test_enabled = false;
        }
    }
    
    inline void EnableDepthWrites()
    {
        if (!m_depth_writes_enabled)
        {
            glDepthMask(GL_TRUE);
            m_depth_writes_enabled = true;
        }
    }
    
    inline void DisableDepthWrites()
    {
        if (m_depth_writes_enabled)
        {
            glDepthMask(GL_FALSE);
            m_depth_writes_enabled = false;
        }
    }
    
    inline void EnableAlphaBlending()
    {
        if (!m_alpha_blending_enabled)
        {
            glEnable(GL_BLEND);
            m_alpha_blending_enabled = true;
        }
    }
    
    inline void DisableAlphaBlending()
    {
        if (m_alpha_blending_enabled)
        {
            glDisable(GL_BLEND);
            m_alpha_blending_enabled = false;
        }
    }
    
    inline void BindArrayBuffer(GLuint array_buffer_name)
    {
        if (m_bound_array_buffer != array_buffer_name)
        {
            glBindBuffer(GL_ARRAY_BUFFER, array_buffer_name);
            m_bound_array_buffer = array_buffer_name;
        }
    }
    
    inline void BindElementArrayBuffer(GLuint element_array_buffer_name)
    {
        if (m_bound_element_array_buffer != element_array_buffer_name)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_name);
            m_bound_element_array_buffer = element_array_buffer_name;
        }
    }
    
    inline void BindProgram(GLuint program_name)
    {
        if (m_bound_program != program_name)
        {
            glUseProgram(program_name);
            m_bound_program = program_name;
        }
    }
    
    inline void BindTexture(GLenum texture_type, GLuint texture_name, GLuint texture_unit)
    {
        SDL_assert(texture_unit < m_bound_textures.size());
        if (m_bound_textures[texture_unit] != texture_name)
        {
            ActivateTextureUnit(texture_unit);
            glBindTexture(texture_type, texture_name);
            m_bound_textures[texture_unit] = texture_name;
        }
    }
    
    inline void ActivateTextureUnit(GLuint texture_unit)
    {
        if (m_active_texture_unit != texture_unit)
        {
            glActiveTexture(GL_TEXTURE0 + texture_unit);
            m_active_texture_unit = texture_unit;
        }
    }
    
    inline void EnableVertexAttribArray(GLuint index)
    {
        SDL_assert(index < m_vertex_attrib_array_states.size());
        if (!m_vertex_attrib_array_states[index])
        {
            glEnableVertexAttribArray(index);
            m_vertex_attrib_array_states[index] = true;
        }
    }
    
    inline void DisableVertexAttribArray(GLuint index)
    {
        SDL_assert(index < m_vertex_attrib_array_states.size());
        if (m_vertex_attrib_array_states[index])
        {
            glDisableVertexAttribArray(index);
            m_vertex_attrib_array_states[index] = false;
        }
    }
};

inline GraphicsDevice* graphics_device()
{
    return app()->graphics_device();
}
