#pragma once

#include <cstddef>

#include <ovis/graphics/graphics_buffer.hpp>

namespace ovis {

enum class IndexFormat {
  UINT8 = GL_UNSIGNED_BYTE,
  UINT16 = GL_UNSIGNED_SHORT,
  UINT32 = GL_UNSIGNED_INT,
};

struct IndexBufferDescription {
  std::size_t size_in_bytes;
  IndexFormat index_format;
};

class IndexBuffer final : public GraphicsBuffer {
  friend class GraphicsContext;

 public:
  IndexBuffer(GraphicsContext* context, const IndexBufferDescription& description, const void* index_data = nullptr);

  virtual ~IndexBuffer() override;

  void Write(std::size_t offset_in_bytes, std::size_t length_in_bytes, const void* index_data);

  inline const IndexBufferDescription& description() const { return m_description; }

  inline std::size_t bytes_per_index() const { return m_bytes_per_index; }

 private:
  IndexBufferDescription m_description;
  std::size_t m_bytes_per_index;

  void Bind();
};

}  // namespace ovis
