#include <vector>
#include <iostream>

#include "application.hpp"
#include "file.hpp"
#include "graphics_device.hpp"
#include "range.hpp"
#include "shader_program.hpp"

ShaderProgram::ShaderProgram
(
    GraphicsDevice* graphics_device,
    const ShaderProgramDescription& description
) :
    GraphicsResource(graphics_device),
    m_description(description),
    m_program_name(glCreateProgram())
{
    AttachShader(description.vertex_shader_source, GL_VERTEX_SHADER);
    AttachShader(description.fragment_shader_source, GL_FRAGMENT_SHADER);
    
    glLinkProgram(m_program_name);
        
    GLint link_status = 0;
    glGetProgramiv(m_program_name, GL_LINK_STATUS, &link_status);
    SDL_assert(link_status == 1);
    
    {
        GLint num_attributes = 0;
        glGetProgramiv(m_program_name, GL_ACTIVE_ATTRIBUTES, &num_attributes);
        
        GLint max_attribute_name_length = 0;
        glGetProgramiv(m_program_name, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attribute_name_length);
        
        std::vector<GLchar> attribute_name_buffer(max_attribute_name_length, '\0');
        
        for (auto i : IRange(num_attributes))
        {
            GLint size = 0;
            GLenum type;
            glGetActiveAttrib
            (
                m_program_name,
                i,
                max_attribute_name_length,
                nullptr,
                &size,
                &type,
                attribute_name_buffer.data()
            );
            
            GLint attribute_location = glGetAttribLocation(m_program_name, attribute_name_buffer.data());
            SDL_assert(attribute_location >= 0);
            
            m_attribute_names[attribute_location] = attribute_name_buffer.data();
            m_attribute_locations[attribute_name_buffer.data()] = attribute_location;
        }
    }
    
    m_uniform_buffer = std::make_unique<UniformBuffer>(graphics_device, UniformBufferDescription{ this });
}

ShaderProgram::~ShaderProgram()
{
    if (graphics_device()->m_bound_program == m_program_name)
    {
        graphics_device()->BindProgram(0);
    }
    
    glDeleteProgram(m_program_name);
}

void ShaderProgram::AttachShader
(
    const std::string &source,
    GLenum shader_type
)
{
    if (source.length() > 0)
    {
        GLuint shader = glCreateShader(shader_type);
        SDL_assert(shader != 0);
        const GLchar* shader_source = source.c_str();
        glShaderSource(shader, 1, &shader_source, nullptr);
        glCompileShader(shader);
        
        GLint compile_status = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        
        if (compile_status == 1)
        {
            glAttachShader(m_program_name, shader);
        } else {
            GLint info_log_length = 0;
            glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &info_log_length);
            std::vector<GLchar> info_log_buffer(info_log_length, '\0');
            glGetShaderInfoLog(shader, info_log_length, nullptr, info_log_buffer.data());
            std::cerr << info_log_buffer.data() << std::endl;
        }
        
        glDeleteShader(shader);
    }
}
    
GLint ShaderProgram::GetAttributeLocation(const std::string& attribute_name)
{
    auto attribute_iterator = m_attribute_locations.find("a_" + attribute_name);
    if (attribute_iterator == m_attribute_locations.end())
    {
        return -1;
    } else
    {
        return attribute_iterator->second;
    }
}

std::unique_ptr<ShaderProgram> LoadShaderProgram(const std::string& path)
{
    ShaderProgramDescription sp_desc;
    sp_desc.vertex_shader_source = LoadTextFile(path + ".vert");
    sp_desc.fragment_shader_source = LoadTextFile(path + ".frag");
    return std::make_unique<ShaderProgram>(graphics_device(), sp_desc);
}

void ShaderProgram::Bind()
{
    graphics_device()->BindProgram(m_program_name);
    m_uniform_buffer->Bind();
}
