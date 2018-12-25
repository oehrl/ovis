#pragma once

#include <array>
#include <fstream>
#include <functional>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <ovis/core/down_cast.hpp>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/core/resource.hpp>

namespace ovis {

class ResourceManager;

using ResourceLoadingFunction =
    std::function<bool(ResourceManager*, const rapidjson::Document&,
                       const std::string&, const std::string&)>;

class ResourceManager {
 public:
  void RegisterFileLoader(const std::string& extension,
                          const ResourceLoadingFunction& loading_function);

  template <typename T = ResourceBase>
  ResourcePointer<T> Load(const std::string& filename);

  template <typename T, typename... Args>
  bool RegisterResource(const std::string& id, Args&&... constructor_arguments);

  template <typename T = ResourceBase>
  ResourcePointer<T> GetResource(const std::string& id);

  void AddSearchPath(std::string path);

 private:
  // std::mutex resources_mutex_;
  std::unordered_multimap<std::string, ResourceLoadingFunction>
      resource_loaders_;
  std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources_;
  std::vector<std::string> search_paths_;
};

template <typename T>
ResourcePointer<T> ResourceManager::Load(const std::string& filename) {
  LogI("Loading '", filename, "'...");
  const std::string extension = filename.substr(filename.find_last_of('.'));
  auto resource_loaders = resource_loaders_.equal_range(extension);
  if (resource_loaders.first == resource_loaders.second) {
    LogE("No file loaders for extension '", extension, "'!");
    return ResourcePointer<T>{};
  }

  if (search_paths_.size() == 0) {
    LogE("No search paths specified!");
  }

  for (const auto& search_path : search_paths_) {
    const std::string resource_path = search_path + filename + ".json";
    std::ifstream resource_parameter_file(resource_path);
    if (!resource_parameter_file.is_open()) {
      LogI("File '", resource_path, "' does not exist");
    } else {
      LogI("File '", resource_path, "' exists");

      rapidjson::IStreamWrapper iws{resource_parameter_file};
      rapidjson::Document parameter_document;
      parameter_document.ParseStream(iws);

      bool is_loaded = false;
      for (auto it = resource_loaders.first;
           it != resource_loaders.second && !is_loaded; ++it) {
        const std::string resource_directory = ExtractDirectory(resource_path);
        if (it->second(this, parameter_document, filename,
                       resource_directory)) {
          is_loaded = true;
          break;
        }
      }

      if (is_loaded) {
        LogI(resource_path, " was successfully loaded!");
      } else {
        LogE("Could not load '", resource_path, "'!");
      }
      break;
    }
  }
  return GetResource(filename);
}

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
ResourcePointer<T> ResourceManager::GetResource(const std::string& id) {
  auto resource_iterator = resources_.find(id);
  if (resource_iterator == resources_.end()) {
    return ResourcePointer<T>{};
  } else {
    return ResourcePointer<T>{
        down_cast<Resource<T>>(resource_iterator->second)};
  }
}

}  // namespace ovis