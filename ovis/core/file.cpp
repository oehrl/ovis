#include <fstream>
#include <vector>

#include <ovis/core/file.hpp>

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
  std::ifstream file(filename, std::ios::binary);

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

bool WriteTextFile(const std::string& filename, const std::string& content) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    return false;
  } else {
    file.write(content.data(), content.size());
    return true;
  }
}

bool WriteBinaryFile(const std::string& filename, const Blob& content) {
  std::ofstream file(filename, std::ios::binary | std::ios::trunc);

  if (!file.is_open()) {
    return false;
  } else {
    file.write(reinterpret_cast<const char*>(content.data()), content.size());
    return true;
  }
}

}  // namespace ovis
