#pragma once

#include <array>
#include <functional>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <ovis/core/down_cast.hpp>
#include <ovis/core/resource.hpp>

namespace ovis {

class ResourceManager;

using ResourceLoadingFunction =
    std::function<bool(ResourceManager*, const std::string&)>;

class ResourceManager {
 public:
  void RegisterFileLoader(const std::string& extension,
                          const ResourceLoadingFunction& loading_function);

  void Load(const std::string& filename);

  template <typename T, typename... Args>
  bool RegisterResource(const std::string& id, Args&&... constructor_arguments);

  template <typename T>
  std::shared_ptr<Resource<T>> GetResource(const std::string& id);

 private:
  // std::mutex resources_mutex_;
  std::unordered_multimap<std::string, ResourceLoadingFunction>
      resource_loaders_;
  std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources_;
};

template <typename T, typename... Args>
bool ResourceManager::RegisterResource(const std::string& id,
                                       Args&&... constructor_arguments) {
  // std::lock_guard<std::mutex> lock(resources_mutex_);
  if (resources_.count(id) == 0) {
    auto resource = std::make_shared<Resource<T>>();
    resource->Create(std::forward<Args>(constructor_arguments)...);
    return resources_.try_emplace(id, std::move(resource)).second;
  } else {
    return false;
  }
}

template <typename T>
std::shared_ptr<Resource<T>> ResourceManager::GetResource(
    const std::string& id) {
  auto resource_iterator = resources_.find(id);
  if (resource_iterator == resources_.end()) {
    return std::shared_ptr<Resource<T>>{};
  } else {
    return down_cast<Resource<T>>(resource_iterator->second);
  }
}

}  // namespace ovis