#include <ovis/core/resource_manager.hpp>

namespace ovis {

void ResourceManager::RegisterFileLoader(const std::string& extension,
                                         const ResourceLoadingFunction& loading_function) {
  resource_loaders_.insert({extension, loading_function});
}

void ResourceManager::AddSearchPath(std::string path) {
  search_paths_.emplace_back(std::move(path));
}

}  // namespace ovis