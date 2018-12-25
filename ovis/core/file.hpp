#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace ovis {

std::string ExtractDirectory(const std::string& file_path);
std::string LoadTextFile(const std::string& filename);
std::vector<std::uint8_t> LoadBinaryFile(const std::string& filename);

}  // namespace ovis
