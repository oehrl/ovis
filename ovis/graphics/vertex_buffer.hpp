#pragma once

#include <cstddef>

#include <ovis/graphics/graphics_buffer.hpp>

namespace ovis {

struct VertexBufferDescription {
  std::size_t size_in_bytes;
  std::size_t vertex_size_in_bytes;
};

class VertexBuffer final : public GraphicsBuffer {
  friend class VertexInput;

 public:
  VertexBuffer(GraphicsContext* context, const VertexBufferDescription& description, const void* vertex_data = nullptr);

  virtual ~VertexBuffer() override;

  void Write(std::size_t offset_in_bytes, std::size_t length_in_bytes, const void* vertex_data);

  inline const VertexBufferDescription& description() const { return m_description; }

 private:
  VertexBufferDescription m_description;

  void Bind();
};

}  // namespace ovis
