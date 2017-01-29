#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_opengles2.h"

#include "graphics_resource.hpp"
#include "uniform_buffer.hpp"

struct ShaderProgramDescription
{
    std::string vertex_shader_source;
    std::string fragment_shader_source;
};

class ShaderProgram :
    public GraphicsResource
{
    friend class UniformBuffer;
    friend class VertexSource;
    friend class GraphicsDevice;

public:
    ShaderProgram
    (
        GraphicsDevice* graphics_device,
        const ShaderProgramDescription& description
    );
    
    virtual ~ShaderProgram() override;
    
    inline const ShaderProgramDescription& description() const
    {
        return m_description;
    }
    
    template <typename... T>
    inline void SetUniform(const std::string& uniform_name, T&&... value)
    {
        m_uniform_buffer->SetUniform(uniform_name, value...);
    }
    
    template <typename... T>
    inline void SetUniform(std::size_t uniform_index, T&&... value)
    {
        m_uniform_buffer->SetUniform(uniform_index, value...);
    }
    
    inline void SetTexture(const std::string& sampler_name, Texture2D* texture)
    {
        m_uniform_buffer->SetTexture(sampler_name, texture);
    }

private:
    ShaderProgramDescription m_description;
    GLuint m_program_name;
    std::unordered_map<std::string, GLint> m_attribute_locations;
    std::unordered_map<GLint, std::string> m_attribute_names;
    std::unique_ptr<UniformBuffer> m_uniform_buffer;
    
    void AttachShader(const std::string& source, GLenum shader_type);
    GLint GetAttributeLocation(const std::string& attribute_name);
    void Bind();
};

std::unique_ptr<ShaderProgram> LoadShaderProgram(const std::string& path);
