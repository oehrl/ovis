#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ovis/core/log.hpp>
#include <ovis/graphics/cubemap.hpp>
#include <ovis/graphics/graphics_resource.hpp>
#include <ovis/graphics/texture2d.hpp>

namespace ovis {

class ShaderProgram;

struct UniformBufferDescription {
  ShaderProgram* shader_program;
};

class UniformBuffer : public GraphicsResource {
  friend class GraphicsDevice;
  friend class ShaderProgram;

  struct UniformDesciption {
    std::size_t offset;
    GLsizei size;
    GLint location;
    GLint base_texture_unit;
    GLenum type;
    std::string name;
  };

 public:
  UniformBuffer(GraphicsContext* context, const UniformBufferDescription& description);
  virtual ~UniformBuffer() override;

  template <typename... T>
  inline void SetUniform(const std::string& name, T&&... value) {
    if (m_uniform_indices.find(name) == m_uniform_indices.end()) {
      LogW("Trying to set unknown uniform: '{}'", name);
      return;
    }
    SetUniform(m_uniform_indices[name], value...);
  }

  inline void SetUniform(std::size_t uniform_index, float value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT);
    memcpy(GetUniformPointer(uniform_index), &value, sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::vec2& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_VEC2);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::vec3& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_VEC3);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::vec4& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_VEC4);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::mat2& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_MAT2);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::mat3& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_MAT3);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::mat4& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_FLOAT_MAT4);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, Sint32 value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_INT);
    memcpy(GetUniformPointer(uniform_index), &value, sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::ivec2& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_INT_VEC2);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::ivec3& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_INT_VEC3);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::ivec4& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_INT_VEC4);
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(value), sizeof(value));
  }

  inline void SetUniform(std::size_t uniform_index, bool value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_BOOL);
    GLint val = value;
    memcpy(GetUniformPointer(uniform_index), &val, sizeof(val));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::bvec2& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_BOOL_VEC2);
    glm::ivec2 val = value;
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(val), sizeof(val));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::bvec3& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_BOOL_VEC3);
    glm::ivec3 val = value;
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(val), sizeof(val));
  }

  inline void SetUniform(std::size_t uniform_index, const glm::bvec4& value) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[uniform_index].type == GL_BOOL_VEC4);
    glm::ivec4 val = value;
    memcpy(GetUniformPointer(uniform_index), glm::value_ptr(val), sizeof(val));
  }

  inline void SetTexture(const std::string& sampler_name, Texture2D* texture) {
    SDL_assert(m_uniform_indices.find(sampler_name) != m_uniform_indices.end());
    SDL_assert(m_uniform_indices[sampler_name] < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[m_uniform_indices[sampler_name]].type == GL_SAMPLER_2D);
    SetTexture(m_uniform_descriptions[m_uniform_indices[sampler_name]].base_texture_unit, texture);
  }

  inline void SetTexture(const std::string& sampler_name, Cubemap* texture) {
    SDL_assert(m_uniform_indices.find(sampler_name) != m_uniform_indices.end());
    SDL_assert(m_uniform_indices[sampler_name] < m_uniform_descriptions.size());
    SDL_assert(m_uniform_descriptions[m_uniform_indices[sampler_name]].type == GL_SAMPLER_CUBE);
    SetTexture(m_uniform_descriptions[m_uniform_indices[sampler_name]].base_texture_unit, texture);
  }

  inline void SetTexture(const std::size_t sampler_index, Texture* texture) {
    SDL_assert(sampler_index < m_textures.size());
    m_textures[sampler_index] = texture;
  }

 private:
  std::unordered_map<std::string, std::size_t> m_uniform_indices;
  std::vector<UniformDesciption> m_uniform_descriptions;
  std::vector<GLbyte> m_uniform_buffer;
  std::vector<Texture*> m_textures;

  inline void* GetUniformBufferPointer(std::size_t offset) { return m_uniform_buffer.data() + offset; }

  inline void* GetUniformPointer(std::size_t uniform_index) {
    SDL_assert(uniform_index < m_uniform_descriptions.size());
    return GetUniformBufferPointer(m_uniform_descriptions[uniform_index].offset);
  }

  void Bind();
};

}  // namespace ovis
