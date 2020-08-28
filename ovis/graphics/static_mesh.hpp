#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <ovis/core/log.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/index_buffer.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>

namespace ovis {

class ShaderProgram;

struct StaticMeshDescription {
  size_t vertex_count;
  size_t index_count;
  PrimitiveTopology primitive_topology;
  std::vector<VertexAttributeDescription> vertex_attributes;
};

template <typename VertexType, typename IndexType>
class StaticMesh {
  static_assert(std::is_same<IndexType, std::uint8_t>::value || std::is_same<IndexType, std::uint16_t>::value ||
                std::is_same<IndexType, std::uint32_t>::value);

 public:
  StaticMesh(GraphicsContext* graphics_context, StaticMeshDescription description);

  inline const VertexType* vertices() const { return vertices_.data(); }
  inline VertexType* vertices() { return vertices_.data(); }

  inline const IndexType* indices() const { return indices_.data(); }
  inline IndexType* indices() { return indices_.data(); }

  inline VertexBuffer* vertex_buffer() const { return vertex_buffer_.get(); }
  inline VertexInput* vertex_input() const { return vertex_input_.get(); }
  inline IndexBuffer* index_buffer() const { return index_buffer_.get(); }

  inline void UpdateVertexBuffer(size_t start = 0, size_t count = 0);
  inline void UpdateIndexBuffer(size_t start = 0, size_t count = 0);

  void Draw(ShaderProgram* shader_program);
  void Draw(DrawItem draw_configuration);

  void DrawPart(ShaderProgram* shader_program, Uint32 start, Uint32 count, Uint32 base_vertex = 0);
  void DrawPart(DrawItem draw_configuration, Uint32 start, Uint32 count, Uint32 base_vertex = 0);

 private:
  GraphicsContext* graphics_context_;
  std::vector<VertexType> vertices_;
  std::vector<IndexType> indices_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<VertexInput> vertex_input_;
  std::unique_ptr<IndexBuffer> index_buffer_;
  StaticMeshDescription description_;
};

template <typename VertexType, typename IndexType>
StaticMesh<VertexType, IndexType>::StaticMesh(GraphicsContext* graphics_context, StaticMeshDescription description)
    : graphics_context_(graphics_context),
      vertices_(description.vertex_count),
      indices_(description.index_count),
      description_(description) {
  VertexBufferDescription vb_desc;
  vb_desc.size_in_bytes = sizeof(VertexType) * description.vertex_count;
  vb_desc.vertex_size_in_bytes = sizeof(VertexType);
  vertex_buffer_ = std::make_unique<VertexBuffer>(graphics_context, vb_desc, vertices_.data());

  VertexInputDescription vi_desc;
  vi_desc.vertex_attributes = description.vertex_attributes;
  vi_desc.vertex_buffers = {vertex_buffer_.get()};
  vertex_input_ = std::make_unique<VertexInput>(graphics_context, vi_desc);

  IndexBufferDescription ib_desc;
  ib_desc.size_in_bytes = sizeof(IndexType) * description.index_count;
  if (sizeof(IndexType) == 1) {
    ib_desc.index_format = IndexFormat::UINT8;
  } else if (sizeof(IndexType) == 2) {
    ib_desc.index_format = IndexFormat::UINT16;
  } else if (sizeof(IndexType) == 4) {
    ib_desc.index_format = IndexFormat::UINT32;
  }
  index_buffer_ = std::make_unique<IndexBuffer>(graphics_context, ib_desc, indices_.data());
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::UpdateVertexBuffer(size_t start, size_t count) {
  SDL_assert(start + count <= vertices_.size());
  const size_t offset_in_bytes = start * sizeof(VertexType);
  const size_t length_in_bytes = (count > 0 ? count : vertices_.size()) * sizeof(VertexType);
  LogD("offset: {}", offset_in_bytes);
  LogD("length_in_bytes: {}", length_in_bytes);
  vertex_buffer_->Write(offset_in_bytes, length_in_bytes, &vertices_[start]);
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::UpdateIndexBuffer(size_t start, size_t count) {
  SDL_assert(start + count <= indices_.size());
  const size_t offset_in_bytes = start * sizeof(IndexType);
  const size_t length_in_bytes = (count > 0 ? count : indices_.size()) * sizeof(IndexType);
  LogD("offset: {}", offset_in_bytes);
  LogD("length_in_bytes: {}", length_in_bytes);
  index_buffer_->Write(offset_in_bytes, length_in_bytes, &indices_[start]);
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::Draw(ShaderProgram* shader_program) {
  DrawPart(shader_program, 0, static_cast<Uint32>(indices_.size()));
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::Draw(DrawItem draw_configuration) {
  DrawPart(draw_configuration, 0, static_cast<Uint32>(indices_.size()));
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::DrawPart(ShaderProgram* shader_program, Uint32 start, Uint32 count,
                                                 Uint32 base_vertex) {
  DrawItem draw_item;
  draw_item.shader_program = shader_program;
  DrawPart(std::move(draw_item), start, count, base_vertex);
}

template <typename VertexType, typename IndexType>
void StaticMesh<VertexType, IndexType>::DrawPart(DrawItem draw_configuration, Uint32 start, Uint32 count,
                                                 Uint32 base_vertex) {
  draw_configuration.primitive_topology = description_.primitive_topology;
  draw_configuration.vertex_input = vertex_input();
  draw_configuration.index_buffer = index_buffer();
  draw_configuration.start = start;
  draw_configuration.count = count;
  draw_configuration.base_vertex = base_vertex;
  graphics_context_->Draw(draw_configuration);
}

enum VertexAttribute {
  POSITION2D,
  POSITION3D,
};

template <VertexAttribute...>
struct Vertex;

template <VertexAttribute ATTRIBUTE, VertexAttribute... ATTRIBUTES>
struct Vertex<ATTRIBUTE, ATTRIBUTES...> : public Vertex<ATTRIBUTE>, public Vertex<ATTRIBUTES...> {};

template <>
struct Vertex<VertexAttribute::POSITION2D> {
  glm::vec2 position;
};

template <>
struct Vertex<VertexAttribute::POSITION3D> {
  glm::vec3 position;
};

inline std::vector<VertexAttributeDescription> GetVertexAttributesDescription(
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
  size_t index_count;
  PrimitiveTopology primitive_topology;
};

template <typename VertexType, typename IndexType>
class SimpleMesh;

template <typename IndexType, VertexAttribute... ATTRIBUTES>
class SimpleMesh<Vertex<ATTRIBUTES...>, IndexType> : public StaticMesh<Vertex<ATTRIBUTES...>, IndexType> {
 public:
  SimpleMesh(GraphicsContext* context, const SimpleMeshDescription& description)
      : StaticMesh<Vertex<ATTRIBUTES...>, IndexType>(
            context, {description.vertex_count, description.index_count, description.primitive_topology,
                      GetVertexAttributesDescription({ATTRIBUTES...})}) {}
};

}  // namespace ovis