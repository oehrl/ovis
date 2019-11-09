#include <ovis/core/file.hpp>

#include <fstream>
#include <vector>

namespace ovis {

std::string ExtractDirectory(const std::string& file_path) {
  return file_path.substr(0, file_path.find_last_of('/'));
}

std::optional<std::string> LoadTextFile(const std::string& filename) {
  std::ifstream file(filename);

  if (file.is_open()) {
    file.seekg(0, std::ios::end);

    std::vector<char> buffer(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(buffer.data(), buffer.size());

    return std::string(buffer.data(), buffer.data() + buffer.size());
  } else {
    return {};
  }
}

std::optional<Blob> LoadBinaryFile(const std::string& filename) {
  std::ifstream file(filename);

  if (file.is_open()) {
    file.seekg(0, std::ios::end);

    Blob buffer(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    return buffer;
  } else {
    return {};
  }
}

}  // namespace ovis
