#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/index_buffer.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>

namespace ovis {

class ShaderProgram;

struct StaticMeshDescription {
  size_t vertex_count;
  PrimitiveTopology primitive_topology;
  std::vector<VertexAttributeDescription> vertex_attributes;
};

template <typename VertexType>
class StaticMesh {
 public:
  StaticMesh(GraphicsContext* graphics_context,
             StaticMeshDescription description);

  inline const VertexType* vertices() const { return vertices_.data(); }
  inline VertexType* vertices() { return vertices_.data(); }

  inline VertexBuffer* vertex_buffer() const { return vertex_buffer_.get(); }
  inline VertexInput* vertex_input() const { return vertex_input_.get(); }

  inline void UpdateVertexBuffer(size_t start = 0, size_t count = 0);

  void Draw(ShaderProgram* shader_program);
  void Draw(DrawItem draw_configuration);

 private:
  GraphicsContext* graphics_context_;
  std::vector<VertexType> vertices_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<VertexInput> vertex_input_;
  StaticMeshDescription description_;
};

template <typename VertexType>
StaticMesh<VertexType>::StaticMesh(GraphicsContext* graphics_context,
                                   StaticMeshDescription description)
    : graphics_context_(graphics_context),
      vertices_(description.vertex_count),
      description_(description) {
  VertexBufferDescription vb_desc;
  vb_desc.size_in_bytes = sizeof(VertexType) * description.vertex_count;
  vb_desc.vertex_size_in_bytes = sizeof(VertexType);
  vertex_buffer_ = std::make_unique<VertexBuffer>(graphics_context, vb_desc,
                                                  vertices_.data());

  VertexInputDescription vi_desc;
  vi_desc.vertex_attributes = description.vertex_attributes;
  vi_desc.vertex_buffers = {vertex_buffer_.get()};
  vertex_input_ = std::make_unique<VertexInput>(graphics_context, vi_desc);

  // IndexBufferDescription ib_desc;
  // ib_desc.size_in_bytes = sizeof(IndexType) * description.vertex_count;
  // ib_desc.vertex_size_in_bytes = sizeof(IndexType);
  // vertex_buffer_ =
  //     std::make_shared<IndexBuffer>(graphics_context, ib_desc,
  //     indices_.data());
}

template <typename VertexType>
void StaticMesh<VertexType>::UpdateVertexBuffer(size_t start, size_t count) {
  SDL_assert(start + count <= vertices_.size());
  const size_t offset_in_bytes = start * sizeof(VertexType);
  const size_t length_in_bytes =
      (count > 0 ? count : vertices_.size()) * sizeof(VertexType);
  LogD("offset: {}", offset_in_bytes);
  LogD("length_in_bytes: {}", length_in_bytes);
  vertex_buffer_->Write(offset_in_bytes, length_in_bytes, &vertices_[start]);
}

template <typename VertexType>
void StaticMesh<VertexType>::Draw(ShaderProgram* shader_program) {
  DrawItem draw_item;
  draw_item.shader_program = shader_program;
  Draw(draw_item);
}

template <typename VertexType>
void StaticMesh<VertexType>::Draw(DrawItem draw_configuration) {
  draw_configuration.primitive_topology = description_.primitive_topology;
  draw_configuration.vertex_input = vertex_input_.get();
  draw_configuration.start = 0;
  draw_configuration.count = vertices_.size();
  graphics_context_->Draw(draw_configuration);
}

enum VertexAttribute {
  POSITION2D,
  POSITION3D,
};

template <VertexAttribute...>
struct Vertex;

template <VertexAttribute ATTRIBUTE, VertexAttribute... ATTRIBUTES>
struct Vertex<ATTRIBUTE, ATTRIBUTES...> : public Vertex<ATTRIBUTE>,
                                          public Vertex<ATTRIBUTES...> {};

template <>
struct Vertex<VertexAttribute::POSITION2D> {
  glm::vec2 position;
};

template <>
struct Vertex<VertexAttribute::POSITION3D> {
  glm::vec3 position;
};

std::vector<VertexAttributeDescription> GetVertexAttributesDescription(
    std::initializer_list<VertexAttribute> attributes) {
  size_t current_offset = 0;
  std::vector<VertexAttributeDescription> descriptions;
  descriptions.reserve(attributes.size());

  std::size_t location = 0;
  for (auto attribute : attributes) {
    VertexAttributeDescription attribute_desc;
    attribute_desc.location = location++;
    attribute_desc.buffer_index = 0;
    attribute_desc.offset_in_bytes = current_offset;

    switch (attribute) {
      case VertexAttribute::POSITION2D:
        attribute_desc.type = VertexAttributeType::FLOAT32_VECTOR2;
        break;

      case VertexAttribute::POSITION3D:
        attribute_desc.type = VertexAttributeType::FLOAT32_VECTOR3;
        break;
    }

    descriptions.push_back(attribute_desc);
  }

  return descriptions;
}

struct SimpleMeshDescription {
  size_t vertex_count;
  PrimitiveTopology primitive_topology;
};

template <VertexAttribute... ATTRIBUTES>
class SimpleMesh : public StaticMesh<Vertex<ATTRIBUTES...>> {
 public:
  SimpleMesh(GraphicsContext* context, const SimpleMeshDescription& description)
      : StaticMesh<Vertex<ATTRIBUTES...>>(
            context, {description.vertex_count, description.primitive_topology,
                      GetVertexAttributesDescription({ATTRIBUTES...})}) {}
};

}  // namespace ovis