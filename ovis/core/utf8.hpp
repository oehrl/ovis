#pragma once

#include <algorithm>
#include <cctype>
#include <string>

#include <ovis/core/log.hpp>

namespace ovis {

std::string to_lower(const std::string& string) {
  std::string lowercase_string;
  lowercase_string.resize(string.size());
  std::transform(string.begin(), string.end(), lowercase_string.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return lowercase_string;
}

}  // namespace ovis