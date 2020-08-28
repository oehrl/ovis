#pragma once

#include <string>
#include <vector>

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

class VertexBuffer;
class ShaderProgram;

enum class VertexAttributeType {
  FLOAT32,
  FLOAT32_VECTOR2,
  FLOAT32_VECTOR3,
  FLOAT32_VECTOR4,

  INT8,
  INT8_VECTOR2,
  INT8_VECTOR3,
  INT8_VECTOR4,

  UINT8,
  UINT8_VECTOR2,
  UINT8_VECTOR3,
  UINT8_VECTOR4,

  INT8_NORM,
  INT8_NORM_VECTOR2,
  INT8_NORM_VECTOR3,
  INT8_NORM_VECTOR4,

  UINT8_NORM,
  UINT8_NORM_VECTOR2,
  UINT8_NORM_VECTOR3,
  UINT8_NORM_VECTOR4,

  INT16,
  INT16_VECTOR2,
  INT16_VECTOR3,
  INT16_VECTOR4,

  UINT16,
  UINT16_VECTOR2,
  UINT16_VECTOR3,
  UINT16_VECTOR4,

  INT16_NORM,
  INT16_NORM_VECTOR2,
  INT16_NORM_VECTOR3,
  INT16_NORM_VECTOR4,

  UINT16_NORM,
  UINT16_NORM_VECTOR2,
  UINT16_NORM_VECTOR3,
  UINT16_NORM_VECTOR4,
};

struct VertexAttributeDescription {
  std::size_t location;
  std::size_t offset_in_bytes;
  std::size_t buffer_index;
  VertexAttributeType type;
};

struct VertexInputDescription {
  std::vector<VertexBuffer*> vertex_buffers;
  std::vector<VertexAttributeDescription> vertex_attributes;
};

class VertexInput final : public GraphicsResource {
  friend class GraphicsContext;

  struct AttributeGlDesc {
    const GLvoid* offset;
    GLsizei stride;
    GLuint array_buffer;
    GLenum type;
    GLint size;
    GLboolean normalized;
    GLboolean enabled;
  };

 public:
  VertexInput(GraphicsContext* context, const VertexInputDescription& description);

  virtual ~VertexInput() override;

 private:
  VertexInputDescription m_description;
  std::vector<AttributeGlDesc> m_attribute_gl_descriptions;

  void Bind();
};

}  // namespace ovis
