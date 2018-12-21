#include "ovis/core/resource_manager.hpp"

namespace ovis {

void ResourceManager::RegisterFileLoader(
    const std::string& extension,
    const ResourceLoadingFunction& loading_function) {
  resource_loaders_.insert({extension, loading_function});
}

void ResourceManager::Load(const std::string& filename) {
  const std::string extension = filename.substr(filename.find_last_of('.'));
  auto range = resource_loaders_.equal_range(extension);
  for (auto it = range.first; it != range.second; ++it) {
    if (it->second(this, filename)) {
      break;
    }
  }
}

}  // namespace ovis