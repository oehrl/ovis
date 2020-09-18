#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ovis {

using Blob = std::vector<std::byte>;

std::string ExtractDirectory(const std::string& file_path);
std::optional<std::string> LoadTextFile(const std::string& filename);
std::optional<Blob> LoadBinaryFile(const std::string& filename);

bool WriteTextFile(const std::string& filename, const std::string& content);
bool WriteBinaryFile(const std::string& filename, const Blob& content);

}  // namespace ovis
