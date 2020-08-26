#include <filesystem>

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_surface.h>

#include <ovis/core/log.hpp>
#include <ovis/core/resource_manager.hpp>

#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/texture2d.hpp>

namespace ovis {

Texture2D::Texture2D(GraphicsContext* context,
                     const Texture2DDescription& description)
    : Texture2D(context, description, nullptr, 0) {}

Texture2D::Texture2D(GraphicsContext* context,
                     const Texture2DDescription& description,
                     const void* pixels, size_t size_in_bytes)
    : Texture(context), description_(description) {
  GLenum internal_format;
  GLenum source_format;
  GLenum source_type;
  Initialize(&internal_format, &source_format, &source_type);
  if (IsTextureFormatCompressed(description.format)) {
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internal_format,
                           static_cast<GLsizei>(description.width),
                           static_cast<GLsizei>(description.height), 0,
                           size_in_bytes, pixels);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
                 static_cast<GLsizei>(description.width),
                 static_cast<GLsizei>(description.height), 0, source_format,
                 source_type, pixels);
  }
}

Texture2D::Texture2D(GraphicsContext* context,
                     const Texture2DDescription& description,
                     const std::vector<Blob>& mip_map_data)
    : Texture(context), description_(description) {
  SDL_assert(description.mip_map_count == mip_map_data.size());
  GLenum internal_format;
  GLenum source_format;
  GLenum source_type;
  Initialize(&internal_format, &source_format, &source_type);

  if (IsTextureFormatCompressed(description.format)) {
    for (size_t mip_level = 0; mip_level < description.mip_map_count;
         ++mip_level) {
      SDL_assert(glGetError() == GL_NO_ERROR);
      glCompressedTexImage2D(GL_TEXTURE_2D, mip_level, internal_format,
                             static_cast<GLsizei>(description.width >> mip_level),
                             static_cast<GLsizei>(description.height >> mip_level), 0,
                             mip_map_data[mip_level].size(),
                             mip_map_data[mip_level].data());
      auto error = glGetError();
      SDL_assert(error == GL_NO_ERROR);
    }
  } else {
    for (size_t mip_level = 0; mip_level < description.mip_map_count;
         ++mip_level) {
      glTexImage2D(GL_TEXTURE_2D, mip_level, internal_format,
                   static_cast<GLsizei>(description.width),
                   static_cast<GLsizei>(description.height), 0, source_format,
                   source_type, mip_map_data[mip_level].data());
    }
  }
}

void Texture2D::Write(std::size_t level, std::size_t x, std::size_t y,
                      std::size_t width, std::size_t height, const void* data) {
  Bind(0);

  GLenum source_format;
  GLenum source_type;
  switch (description_.format) {
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

void Texture2D::Initialize(GLenum* internal_format, GLenum* source_format,
                           GLenum* source_type) {
  Bind(0);

  switch (description_.format) {
    case TextureFormat::RGB_UINT8:
      *internal_format = GL_RGB;
      *source_format = GL_RGB;
      *source_type = GL_UNSIGNED_BYTE;
      break;

    case TextureFormat::RGBA_UINT8:
      *internal_format = GL_RGBA;
      *source_format = GL_RGBA;
      *source_type = GL_UNSIGNED_BYTE;
      break;

    case TextureFormat::RGBA_S3TC_DXT1:
      *internal_format = 0x83F0;
      break;

    case TextureFormat::RGBA_S3TC_DXT5:
      *internal_format = 0x83F3;
      break;

    case TextureFormat::RGBA_FLOAT32:
      *internal_format = GL_RGBA32F;
      *source_format = GL_RGBA;
      *source_type = GL_FLOAT;
      break;

    case TextureFormat::DEPTH_UINT16:
      *internal_format = GL_DEPTH_COMPONENT16;
      *source_format = GL_DEPTH_COMPONENT;
      *source_type = GL_FLOAT;
      break;

    case TextureFormat::DEPTH_UINT24:
      *internal_format = GL_DEPTH_COMPONENT24;
      *source_format = GL_DEPTH_COMPONENT;
      *source_type = GL_FLOAT;
      break;

    case TextureFormat::DEPTH_FLOAT32:
      *internal_format = GL_DEPTH_COMPONENT32F;
      *source_format = GL_DEPTH_COMPONENT;
      *source_type = GL_FLOAT;
      break;

    default:
      SDL_assert(false);
      break;
  }

  GLenum min_filter;
  GLenum mag_filter;
  switch (description_.filter) {
    case TextureFilter::POINT:
      min_filter = description_.mip_map_count > 1 ? GL_NEAREST_MIPMAP_NEAREST
                                                  : GL_NEAREST;
      mag_filter = GL_NEAREST;
      break;

    case TextureFilter::BILINEAR:
      min_filter =
          description_.mip_map_count > 1 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
      mag_filter = GL_LINEAR;
      break;

    case TextureFilter::TRILINEAR:
      // SDL_assert(description_.mip_map_count > 1);
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
  texture2d_desc.mip_map_count = parameters["mip_level_data_files"].Size();

  const auto& filter = parameters["filter"].GetString();
  if (std::strcmp(filter, "point") == 0) {
    texture2d_desc.filter = TextureFilter::POINT;
  } else if (std::strcmp(filter, "bilinear") == 0) {
    texture2d_desc.filter = TextureFilter::BILINEAR;
  } else if (std::strcmp(filter, "trilinear") == 0) {
    texture2d_desc.filter = TextureFilter::TRILINEAR;
  } else {
    LogE("Failed to load texture '{}': invalid filter ()", id, filter);
    return false;
  }

  const auto& format = parameters["format"].GetString();
  if (std::strcmp(format, "RGB_UINT8") == 0) {
    texture2d_desc.format = TextureFormat::RGB_UINT8;
  } else if (std::strcmp(format, "RGBA_UINT8") == 0) {
    texture2d_desc.format = TextureFormat::RGBA_UINT8;
  } else if (std::strcmp(format, "S3TC_DXT1") == 0) {
    texture2d_desc.format = TextureFormat::RGBA_S3TC_DXT1;
  } else if (std::strcmp(format, "S3TC_DXT5") == 0) {
    texture2d_desc.format = TextureFormat::RGBA_S3TC_DXT5;
  } else {
    LogE("Failed to load texture '{}': invalid format ({})", id, format);
    return false;
  }

  std::vector<Blob> mip_map_data;
  for (rapidjson::SizeType i = 0; i < texture2d_desc.mip_map_count; ++i) {
    std::string filename = parameters["mip_level_data_files"][i].GetString();
    auto data = LoadBinaryFile(std::filesystem::path(id).parent_path().string() + "/" + filename);
    if (!data.has_value()) {
      LogE("Failed to load MIP level {} of texture '{}' ({})", i, id, filename);
    } else {
      mip_map_data.push_back(std::move(*data));
    }
  }

  resource_manager->RegisterResource<Texture2D>(id, graphics_context,
                                                texture2d_desc, mip_map_data);
  LogI("Sucessfully loaded texture: {}", id);
  return true;
}

}  // namespace ovis
