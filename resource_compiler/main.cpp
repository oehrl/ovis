#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stb_image.h>

int CompileTexture2D(std::filesystem::path input_file,
                     std::filesystem::path output_directory) {
  namespace fs = std::filesystem;

  const fs::path output_data_file = output_directory / input_file.stem() +=
      ".texture2d";
  const fs::path output_parameter_file = output_directory / input_file.stem() +=
      ".texture2d.json";

  const auto input_write_time = fs::last_write_time(input_file);

  if (fs::exists(output_data_file) &&
      input_write_time < fs::last_write_time(output_data_file) &&
      fs::exists(output_parameter_file) &&
      input_write_time < fs::last_write_time(output_parameter_file)) {
    std::cout << "  Nothing to do\n";
    return 0;
  }

  int image_width;
  int image_height;
  int image_channel_count;

  unsigned char* data = stbi_load(input_file.c_str(), &image_width,
                                  &image_height, &image_channel_count, 0);

  if (data) {
    std::ofstream file(output_data_file.c_str(), std::ios::binary);
    if (!file.is_open()) {
      stbi_image_free(data);
      std::cerr << "  Cannot open " << output_data_file << std::endl;
      return -3;
    }
    file.write(reinterpret_cast<const char*>(data),
               image_width * image_height * image_channel_count);
    file.close();
    stbi_image_free(data);
    std::cout << "  Wrote " << output_data_file << "\n";

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
    d.AddMember(
        "data_file",
        rapidjson::Value(output_data_file.filename().c_str(), allocator),
        allocator);
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);

    std::ofstream params_file(output_parameter_file.c_str());
    if (!params_file.is_open()) {
      std::cerr << "  Cannot open " << output_parameter_file << std::endl;
      return -3;
    }

    params_file << strbuf.GetString();
    params_file.close();
    std::cout << "  Wrote " << output_parameter_file << "\n";
  } else {
    std::cerr << "  Failed to load file: " << input_file.c_str() << std::endl;
    return -2;
  }

  return 0;
}

int CompileShader(std::filesystem::path input_file,
                  std::filesystem::path output_directory) {
  namespace fs = std::filesystem;

  const auto vertex_shader = input_file.parent_path() / input_file.stem() +=
      ".vert";
  const auto fragment_shader = input_file.parent_path() / input_file.stem() +=
      ".frag";

  if (!fs::exists(vertex_shader)) {
    std::cerr << "  Cannot find vertex shader: " << vertex_shader << std::endl;
    return -1;
  } else if (!fs::exists(fragment_shader)) {
    std::cerr << "  Cannot find fragment shader: " << fragment_shader
              << std::endl;
    return -1;
  }

  const auto last_input_vertex_shader_write_time =
      fs::last_write_time(vertex_shader);
  const auto last_input_fragment_shader_write_time =
      fs::last_write_time(fragment_shader);
  const auto last_input_write_time =
      std::max(last_input_vertex_shader_write_time,
               last_input_fragment_shader_write_time);

  const auto output_vertex_shader_filename =
      output_directory / vertex_shader.filename();
  const auto output_fragment_shader_filename =
      output_directory / fragment_shader.filename();
  const auto output_parameter_filename = output_directory / input_file.stem() +=
      ".shader.json";

  bool did_something = false;
  if (!fs::exists(output_vertex_shader_filename) ||
      fs::last_write_time(output_vertex_shader_filename) <
          last_input_vertex_shader_write_time) {
    if (fs::copy_file(vertex_shader, output_vertex_shader_filename)) {
      std::cout << "  Wrote " << output_vertex_shader_filename << "\n";
    } else {
      std::cerr << "  Failed to write " << output_vertex_shader_filename
                << "\n";
      return -3;
    }
    did_something = true;
  }
  if (!fs::exists(output_fragment_shader_filename) ||
      fs::last_write_time(output_fragment_shader_filename) <
          last_input_fragment_shader_write_time) {
    if (fs::copy_file(fragment_shader, output_fragment_shader_filename)) {
      std::cout << "  Wrote " << output_fragment_shader_filename << "\n";
    } else {
      std::cerr << "  Failed to write " << output_fragment_shader_filename
                << "\n";
      return -3;
    }
    did_something = true;
  }
  if (!fs::exists(output_parameter_filename) ||
      fs::last_write_time(output_parameter_filename) < last_input_write_time) {
    rapidjson::Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    d.AddMember(
        "vertex_shader_source",
        rapidjson::Value(output_vertex_shader_filename.filename().c_str(),
                         allocator),
        allocator);
    d.AddMember(
        "fragment_shader_source",
        rapidjson::Value(output_fragment_shader_filename.filename().c_str(),
                         allocator),
        allocator);
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);

    std::ofstream params_file(output_parameter_filename.c_str());
    if (!params_file.is_open()) {
      std::cerr << "  Cannot open " << output_parameter_filename << std::endl;
      return -3;
    }
    params_file << strbuf.GetString();
    params_file.close();
    std::cout << "  Wrote " << output_parameter_filename << "\n";
  }
  if (!did_something) {
    std::cout << "  Nothing to do\n";
  }
  return 0;
}

int main(int argc, char const* argv[]) {
  namespace fs = std::filesystem;

  const std::string usage =
      "OvisResourceCompiler <input_directory> <output_directory>";
  if (argc != 3) {
    std::cerr << usage << std::endl;
    return -1;
  } else {
    std::filesystem::path input_directory(argv[1]);
    std::filesystem::path output_directory(argv[2]);

    if (!fs::is_directory(input_directory)) {
      std::cerr << input_directory << " is not a directory." << std::endl;
      return -2;
    }

    if (!fs::is_directory(output_directory)) {
      if (!fs::create_directories(output_directory)) {
        std::cerr << "Failed to create output directory!" << std::endl;
        return -3;
      }
    }

    std::unordered_set<std::string> handled_files;
    for (auto& p : fs::recursive_directory_iterator(input_directory)) {
      fs::path relative_path = fs::relative(p, input_directory);
      if (p.is_directory()) {
        std::cout << "DIRECTORY: " << relative_path << "\n";

        const auto directory_to_create = output_directory / relative_path;
        if (fs::exists(output_directory)) {
          std::cout << "  Nothing to do\n";
        } else if (!fs::create_directories(directory_to_create)) {
          std::cerr << "  Failed to create output directory!" << std::endl;
          return -3;
        } else {
          std::cout << "  Created " << directory_to_create << "\n";
        }
      } else {
        fs::path current_output_directory =
            output_directory /
            fs::relative(p.path().parent_path(), input_directory);
        const auto extension = p.path().extension();

        if (extension == ".png") {
          std::cout << "TEXTURE2D: " << relative_path << "\n";
          int result = CompileTexture2D(p, current_output_directory);
          if (result != 0) {
            return result;
          }
        } else if (extension == ".vert") {
          std::cout << "SHADER: "
                    << relative_path.parent_path() / relative_path.stem()
                    << "\n";
          int result = CompileShader(p, current_output_directory);
          if (result != 0) {
            return result;
          }
        }
      }
    }
  }
}