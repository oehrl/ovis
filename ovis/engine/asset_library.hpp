#pragma once

#include <filesystem>
#include <unordered_map>

#include <ovis/core/file.hpp>
#include <ovis/core/json.hpp>

namespace ovis {

class AssetLibrary {
 public:
  AssetLibrary(const std::string& directory);

  bool Contains(const std::string& asset_id) const;
  std::string GetAssetType(const std::string& asset_id) const;
  std::vector<std::string> GetAssetsWithType(const std::string& type) const;
  void Refresh();

 private:
  std::filesystem::path directory_;
  std::unordered_map<std::string, std::filesystem::path> asset_paths_;
  std::unordered_multimap<std::string, std::string> asset_with_type_;
};

}  // namespace ovis
