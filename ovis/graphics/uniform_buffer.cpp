#include <ovis/core/log.hpp>
#include <ovis/core/range.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/uniform_buffer.hpp>

namespace ovis {

namespace {
std::size_t GetUniformSize(GLenum type) {
  switch (type) {
    case GL_FLOAT:
      return 4;
    case GL_FLOAT_VEC2:
      return 4 * 2;
    case GL_FLOAT_VEC3:
      return 4 * 3;
    case GL_FLOAT_VEC4:
      return 4 * 4;

    case GL_INT:
      return 4;
    case GL_INT_VEC2:
      return 4 * 2;
    case GL_INT_VEC3:
      return 4 * 3;
    case GL_INT_VEC4:
      return 4 * 4;

    case GL_BOOL:
      return 4;
    case GL_BOOL_VEC2:
      return 4 * 2;
    case GL_BOOL_VEC3:
      return 4 * 3;
    case GL_BOOL_VEC4:
      return 4 * 4;

    case GL_FLOAT_MAT2:
      return 4 * 2 * 2;
    case GL_FLOAT_MAT3:
      return 4 * 3 * 3;
    case GL_FLOAT_MAT4:
      return 4 * 4 * 4;

    case GL_SAMPLER_2D:
      return 0;
    case GL_SAMPLER_CUBE:
      return 0;

    default:
      SDL_assert(false);
      return 0;
  }
}
}  // namespace

UniformBuffer::UniformBuffer(GraphicsContext* context, const UniformBufferDescription& description)
    : GraphicsResource(context) {
  GLuint program_name = description.shader_program->m_program_name;
  GLint num_uniforms = 0;
  glGetProgramiv(program_name, GL_ACTIVE_UNIFORMS, &num_uniforms);

  GLint max_uniform_name_length = 0;
  glGetProgramiv(program_name, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);

  std::vector<GLchar> uniform_name_buffer(max_uniform_name_length, '\0');

  m_uniform_descriptions.resize(num_uniforms);
  std::size_t current_size = 0;
  GLint texture_count = 0;

  for (auto i : IRange(num_uniforms)) {
    m_uniform_descriptions[i].offset = current_size;

    glGetActiveUniform(program_name, i, max_uniform_name_length, nullptr, &m_uniform_descriptions[i].size,
                       &m_uniform_descriptions[i].type, uniform_name_buffer.data());

    m_uniform_descriptions[i].name = uniform_name_buffer.data();

    m_uniform_descriptions[i].location = glGetUniformLocation(program_name, uniform_name_buffer.data());
    SDL_assert(m_uniform_descriptions[i].location >= 0);

    current_size += GetUniformSize(m_uniform_descriptions[i].type) * m_uniform_descriptions[i].size;

    if (m_uniform_descriptions[i].type == GL_SAMPLER_2D || m_uniform_descriptions[i].type == GL_SAMPLER_CUBE) {
      m_uniform_descriptions[i].base_texture_unit = texture_count;
      texture_count += m_uniform_descriptions[i].size;
    } else {
      m_uniform_descriptions[i].base_texture_unit = -1;
    }

    SDL_assert(uniform_name_buffer[0] == 'u' && uniform_name_buffer[1] == '_');
    m_uniform_indices[uniform_name_buffer.data() + 2] = i;
    LogD("{}={}", uniform_name_buffer.data(), i);
  }

  m_uniform_buffer.resize(current_size, 0);
  m_textures.resize(texture_count, nullptr);
}

UniformBuffer::~UniformBuffer() {}

void UniformBuffer::Bind() {
  for (const auto& uniform_desc : m_uniform_descriptions) {
    const void* const data = GetUniformBufferPointer(uniform_desc.offset);
    const GLint location = uniform_desc.location;
    const GLsizei count = uniform_desc.size;

    switch (uniform_desc.type) {
      case GL_FLOAT:
        glUniform1fv(location, count, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_VEC2:
        glUniform2fv(location, count, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_VEC3:
        glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_VEC4:
        glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_MAT2:
        glUniformMatrix2fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_MAT3:
        glUniformMatrix3fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_FLOAT_MAT4:
        glUniformMatrix4fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(data));
        break;

      case GL_INT:
      case GL_BOOL:
        glUniform1iv(location, count, reinterpret_cast<const GLint*>(data));
        break;

      case GL_INT_VEC2:
      case GL_BOOL_VEC2:
        glUniform2iv(location, count, reinterpret_cast<const GLint*>(data));
        break;

      case GL_INT_VEC3:
      case GL_BOOL_VEC3:
        glUniform3iv(location, count, reinterpret_cast<const GLint*>(data));
        break;

      case GL_INT_VEC4:
      case GL_BOOL_VEC4:
        glUniform4iv(location, count, reinterpret_cast<const GLint*>(data));
        break;

      case GL_SAMPLER_2D:
      case GL_SAMPLER_CUBE:
        SDL_assert(count == 1);
        glUniform1i(location, uniform_desc.base_texture_unit);
        break;
    }
  }

  for (auto it : IndexRange<Uint32>(m_textures)) {
    if (it.value() != nullptr) {
      it.value()->Bind(it.index());
    } else {
      context()->BindTexture(GL_TEXTURE_2D, 0, it.index());
    }
  }
}

}  // namespace ovis
