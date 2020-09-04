#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <ovis/core/json.hpp>
#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>
#include <ovis/graphics/uniform_buffer.hpp>

namespace ovis {

struct ShaderProgramDescription {
  std::string vertex_shader_source;
  std::string fragment_shader_source;
};

class ShaderProgram : public GraphicsResource {
  friend class UniformBuffer;
  friend class VertexInput;
  friend class GraphicsContext;

 public:
  ShaderProgram(GraphicsContext* context, const ShaderProgramDescription& description);

  virtual ~ShaderProgram() override;

  inline const ShaderProgramDescription& description() const { return m_description; }

  template <typename... T>
  inline void SetUniform(const std::string& uniform_name, T&&... value) {
    m_uniform_buffer->SetUniform(uniform_name, value...);
  }

  template <typename... T>
  inline void SetUniform(std::size_t uniform_index, T&&... value) {
    m_uniform_buffer->SetUniform(uniform_index, value...);
  }

  inline void SetTexture(const std::string& sampler_name, Texture2D* texture) {
    m_uniform_buffer->SetTexture(sampler_name, texture);
  }

  inline void SetTexture(const std::string& sampler_name, Cubemap* texture) {
    m_uniform_buffer->SetTexture(sampler_name, texture);
  }

  std::optional<std::size_t> GetAttributeLocation(const std::string& attribute_name);

 private:
  ShaderProgramDescription m_description;
  GLuint m_program_name;
  std::unordered_map<std::string, GLint> m_attribute_locations;
  std::unordered_map<GLint, std::string> m_attribute_names;
  std::unique_ptr<UniformBuffer> m_uniform_buffer;

  void AttachShader(const std::string& source, GLenum shader_type);
  void Bind();
};

bool LoadShaderProgram(GraphicsContext* graphics_context, ResourceManager* resource_manager, const json& parameters,
                       const std::string& id, const std::string& directory);

}  // namespace ovis
