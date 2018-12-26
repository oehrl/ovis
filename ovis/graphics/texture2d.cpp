#include <SDL_assert.h>
#include <SDL_surface.h>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/core/resource_manager.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/texture2d.hpp>

namespace ovis {

Texture2D::Texture2D(GraphicsContext* context,
                     const Texture2DDescription& description,
                     const void* pixels)
    : Texture(context), m_description(description) {
  Bind(0);

  GLenum internal_format;
  GLenum source_format;
  GLenum source_type;
  switch (description.format) {
    case TextureFormat::RGB_UINT8:
      internal_format = GL_RGB;
      source_format = GL_RGB;
      source_type = GL_UNSIGNED_BYTE;
      break;

    case TextureFormat::RGBA_UINT8:
      internal_format = GL_RGBA;
      source_format = GL_RGBA;
      source_type = GL_UNSIGNED_BYTE;
      break;

    default:
      SDL_assert(false);
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
               static_cast<GLsizei>(description.width),
               static_cast<GLsizei>(description.height), 0, source_format,
               source_type, pixels);

  GLenum min_filter;
  GLenum mag_filter;
  switch (description.filter) {
    case TextureFilter::POINT:
      min_filter = description.mip_map_count > 1 ? GL_NEAREST_MIPMAP_NEAREST
                                                 : GL_NEAREST;
      mag_filter = GL_NEAREST;
      break;

    case TextureFilter::BILINEAR:
      min_filter =
          description.mip_map_count > 1 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
      mag_filter = GL_LINEAR;
      break;

    case TextureFilter::TRILINEAR:
      SDL_assert(description.mip_map_count > 1);
      min_filter = GL_LINEAR_MIPMAP_LINEAR;
      mag_filter = GL_LINEAR;
      break;

    default:
      SDL_assert(false);
      break;
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Texture2D::Write(std::size_t level, std::size_t x, std::size_t y,
                      std::size_t width, std::size_t height, const void* data) {
  Bind(0);

  GLenum source_format;
  GLenum source_type;
  switch (m_description.format) {
    case TextureFormat::RGB_UINT8:
      source_format = GL_RGB;
      source_type = GL_UNSIGNED_BYTE;
      break;

    case TextureFormat::RGBA_UINT8:
      source_format = GL_RGBA;
      source_type = GL_UNSIGNED_BYTE;
      break;

    default:
      SDL_assert(false);
      break;
  }

  glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLsizei>(level),
                  static_cast<GLsizei>(x), static_cast<GLsizei>(y),
                  static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                  source_format, source_type, data);
}

void Texture2D::Bind(int texture_unit) {
  context()->BindTexture(GL_TEXTURE_2D, name(), texture_unit);
}

bool LoadTexture2D(GraphicsContext* graphics_context,
                   ResourceManager* resource_manager,
                   const rapidjson::Document& parameters, const std::string& id,
                   const std::string& directory) {
  Texture2DDescription texture2d_desc;
  texture2d_desc.width = parameters["width"].GetInt();
  texture2d_desc.height = parameters["height"].GetInt();
  texture2d_desc.mip_map_count = 1;

  const auto& filter = parameters["filter"].GetString();
  if (std::strcmp(filter, "point") == 0) {
    texture2d_desc.filter = TextureFilter::POINT;
  } else if (std::strcmp(filter, "bilinear") == 0) {
    texture2d_desc.filter = TextureFilter::BILINEAR;
  } else if (std::strcmp(filter, "trilinear") == 0) {
    texture2d_desc.filter = TextureFilter::TRILINEAR;
  } else {
    LogE("Failed to load texture '", id, "': invalid filter (", filter, ")");
    return false;
  }

  const auto& format = parameters["format"].GetString();
  if (std::strcmp(format, "RGB_UINT8") == 0) {
    texture2d_desc.format = TextureFormat::RGB_UINT8;
  } else if (std::strcmp(format, "RGBA_UINT8") == 0) {
    texture2d_desc.format = TextureFormat::RGBA_UINT8;
  } else {
    LogE("Failed to load texture '", id, "': invalid format (", format, ")");
    return false;
  }

  std::vector<uint8_t> buffer =
      LoadBinaryFile(directory + "/" + parameters["data_file"].GetString());
  resource_manager->RegisterResource<Texture2D>(id, graphics_context,
                                                texture2d_desc, buffer.data());

  LogI("Sucessfully loaded texture: ", id);
  return true;
}

}  // namespace ovis
