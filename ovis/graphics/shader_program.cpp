#include <fstream>
#include <iostream>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/core/range.hpp>
#include <ovis/core/resource_manager.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>

namespace ovis {

ShaderProgram::ShaderProgram(GraphicsContext* context,
                             const ShaderProgramDescription& description)
    : GraphicsResource(context),
      m_description(description),
      m_program_name(glCreateProgram()) {
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
    glGetProgramiv(m_program_name, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                   &max_attribute_name_length);

    std::vector<GLchar> attribute_name_buffer(max_attribute_name_length, '\0');

    for (auto i : IRange(num_attributes)) {
      GLint size = 0;
      GLenum type;
      glGetActiveAttrib(m_program_name, i, max_attribute_name_length, nullptr,
                        &size, &type, attribute_name_buffer.data());

      GLint attribute_location =
          glGetAttribLocation(m_program_name, attribute_name_buffer.data());
      SDL_assert(attribute_location >= 0);

      m_attribute_names[attribute_location] = attribute_name_buffer.data();
      m_attribute_locations[attribute_name_buffer.data()] = attribute_location;
    }
  }

  m_uniform_buffer =
      std::make_unique<UniformBuffer>(context, UniformBufferDescription{this});
}

ShaderProgram::~ShaderProgram() {
  if (context()->m_bound_program == m_program_name) {
    glUseProgram(0);
    context()->m_bound_program = 0;
  }

  glDeleteProgram(m_program_name);
}

void ShaderProgram::AttachShader(const std::string& source,
                                 GLenum shader_type) {
  if (source.length() > 0) {
    GLuint shader = glCreateShader(shader_type);
    SDL_assert(shader != 0);

    std::string final_shader_source;
#if OVIS_EMPSCRIPTEN
    final_shader_source += "#version 100\n";
    final_shader_source += "#define OVIS_EMSCRIPTEN 1";
    if (shader_type == GL_VERTEX_SHADER) {
      final_shader_source += "#define in attribute\n";
      final_shader_source += "#define out varying\n";
    } else if (shader_type == GL_FRAGMENT_SHADER) {
      final_shader_source += "precision mediump float;\n";
      final_shader_source += "#define in varying\n";
    }
#else
    final_shader_source += "#version 120\n";
    if (shader_type == GL_VERTEX_SHADER) {
      final_shader_source += "#define in attribute\n";
      final_shader_source += "#define out varying\n";
    } else if (shader_type == GL_FRAGMENT_SHADER) {
      final_shader_source += "#define in varying\n";
    }
#endif
    final_shader_source += source;

    const GLchar* shader_source = final_shader_source.c_str();
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);

    GLint compile_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == 1) {
      glAttachShader(m_program_name, shader);
    } else {
      GLint info_log_length = 0;
      glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &info_log_length);
      std::vector<GLchar> info_log_buffer(info_log_length, '\0');
      glGetShaderInfoLog(shader, info_log_length, nullptr,
                         info_log_buffer.data());
      LogE(info_log_buffer.data());
    }

    glDeleteShader(shader);
  }
}

GLint ShaderProgram::GetAttributeLocation(const std::string& attribute_name) {
  auto attribute_iterator = m_attribute_locations.find("a_" + attribute_name);
  if (attribute_iterator == m_attribute_locations.end()) {
    return -1;
  } else {
    return attribute_iterator->second;
  }
}

void ShaderProgram::Bind() {
  if (context()->m_bound_program != m_program_name) {
    glUseProgram(m_program_name);
    context()->m_bound_program = m_program_name;
  }
  m_uniform_buffer->Bind();
}

bool LoadShaderProgram(GraphicsContext* graphics_context,
                       ResourceManager* resource_manager,
                       const rapidjson::Document& parameters,
                       const std::string& id, const std::string& directory) {
  LogD(directory + "/" + parameters["vertex_shader_source"].GetString());
  ShaderProgramDescription sp_desc;
  sp_desc.vertex_shader_source = LoadTextFile(
      directory + "/" + parameters["vertex_shader_source"].GetString());
  sp_desc.fragment_shader_source = LoadTextFile(
      directory + "/" + parameters["fragment_shader_source"].GetString());

  resource_manager->RegisterResource<ShaderProgram>(id, graphics_context,
                                                    sp_desc);

  LogI("Sucessfully loaded shader program: ", id);
  return true;
}

}  // namespace ovis
