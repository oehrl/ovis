#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ovis {

std::string ExtractDirectory(const std::string& file_path);
std::optional<std::string> LoadTextFile(const std::string& filename);
std::optional<std::vector<std::uint8_t>> LoadBinaryFile(
    const std::string& filename);

}  // namespace ovis
