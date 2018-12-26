#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stb_image.h>

int ParseTexture2D(int argc, const char* argv[]) {
  const std::string usage = "TextureConverter texture2d [in] [out]";
  if (argc < 2) {
    std::cout << usage << std::endl;
    return -1;
  } else {
    int image_width;
    int image_height;
    int image_channel_count;

    unsigned char* data = stbi_load(argv[0], &image_width, &image_height,
                                    &image_channel_count, 0);

    if (data) {
      std::ofstream file(argv[1], std::ios::binary);
      file.write(reinterpret_cast<const char*>(data),
                 image_width * image_height * image_channel_count);
      file.close();
      stbi_image_free(data);

      rapidjson::Document d;
      d.SetObject();
      rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

      // (image_channel_count == 3 ? "RGB_UINT8" : "RGBA_UINT8")
      if (image_channel_count == 3) {
        d.AddMember("format", "RGB_UINT8", allocator);
      } else if (image_channel_count == 4) {
        d.AddMember("format", "RGBA_UINT8", allocator);
      }
      d.AddMember("width", image_width, allocator);
      d.AddMember("height", image_height, allocator);
      d.AddMember("filter", "bilinear", allocator);
      d.AddMember("data_file", rapidjson::Value(argv[1], allocator), allocator);
      rapidjson::StringBuffer strbuf;
      rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
      d.Accept(writer);

      std::string params_filename = argv[1];
      params_filename += ".json";
      std::ofstream params_file(params_filename);
      params_file << strbuf.GetString();
      params_file.close();
    } else {
      std::cerr << "Failed to load file: " << argv[0] << std::endl;
      return -2;
    }

    return 0;
  }
}

int ParseCubemap(int argc, const char* argv[]) {
  const std::string usage =
      "TextureConverter cubemap [right] [left] [top] [bottom] [front] [back] "
      "[out]";
  if (argc < 7) {
    std::cout << usage << std::endl;
    return -1;
  } else {
    int image_width;
    int image_height;
    int image_channel_count;
    std::vector<uint8_t> buffer;
    for (int i = 0; i < 6; ++i) {
      int x, y, channel_count;
      unsigned char* data = stbi_load(argv[i], &x, &y, &channel_count, 0);
      if (data) {
        if (i == 0) {
          image_width = x;
          image_height = y;
          image_channel_count = channel_count;
          buffer.reserve(x * y * channel_count * 6);
        } else {
          if (x != image_width || y != image_height ||
              channel_count != image_channel_count) {
            std::cerr << "The image " << argv[i]
                      << "has different dimensions: " << x << "x" << y << "x"
                      << channel_count << ".\n"
                      << "Should be: " << image_width << "x" << image_height
                      << "x" << image_channel_count << std::endl;
            stbi_image_free(data);
            return -2;
          }
        }
        buffer.insert(buffer.end(), data, data + (x * y * channel_count));
        stbi_image_free(data);
      } else {
        std::cerr << "Failed to load file: " << argv[i] << std::endl;
        return -2;
      }
    }

    std::ofstream file(argv[6], std::ios::binary);
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    file.close();

    rapidjson::Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    // (image_channel_count == 3 ? "RGB_UINT8" : "RGBA_UINT8")
    if (image_channel_count == 3) {
      d.AddMember("format", "RGB_UINT8", allocator);
    } else if (image_channel_count == 4) {
      d.AddMember("format", "RGBA_UINT8", allocator);
    }
    d.AddMember("width", image_width, allocator);
    d.AddMember("height", image_height, allocator);
    d.AddMember("filter", "bilinear", allocator);
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);

    std::string params_filename = argv[6];
    params_filename += ".json";
    std::ofstream params_file(params_filename);
    params_file << strbuf.GetString();
    params_file.close();

    return 0;
  }
}

int main(int argc, char const* argv[]) {
  const std::string usage =
      "TextureConverter [2d|cubemap] <type specific options>";
  if (argc < 2) {
    std::cout << usage << std::endl;
    return -1;
  } else {
    const char* type = argv[1];
    if (std::strcmp(type, "2d") == 0) {
      return ParseTexture2D(argc - 2, argv + 2);
    } else if (std::strcmp(type, "cubemap") == 0) {
      return ParseCubemap(argc - 2, argv + 2);
    } else {
      std::cout << usage << std::endl;
      return -1;
    }
  }
}
