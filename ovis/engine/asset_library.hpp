#pragma once

#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <variant>

#include <ovis/core/file.hpp>
#include <ovis/core/json.hpp>

namespace ovis {

class AssetLibrary {
 public:
  virtual ~AssetLibrary() = default;

  virtual bool Contains(const std::string& asset_id) const = 0;
  virtual std::vector<std::string> GetAssets() const = 0;
  virtual std::string GetAssetType(const std::string& asset_id) const = 0;
  virtual std::vector<std::string> GetAssetFileTypes(const std::string& asset_id) const = 0;
  virtual std::optional<std::string> LoadAssetTextFile(const std::string& asset_id,
                                                       const std::string& file_type) const = 0;
  virtual std::optional<Blob> LoadAssetBinaryFile(const std::string& asset_id, const std::string& file_type) const = 0;
  virtual std::vector<std::string> GetAssetsWithType(const std::string& type) const = 0;

  virtual bool CreateAsset(const std::string& asset_id, const std::string& type,
                           const std::vector<std::pair<std::string, std::variant<std::string, Blob>>>& files) {
    return false;
  }

  virtual bool SaveAssetFile(const std::string& asset_id, const std::string& file_type,
                             std::variant<std::string, Blob> content) {
    return false;
  }
};

class DirectoryAssetLibrary : public AssetLibrary {
 public:
  DirectoryAssetLibrary(const std::string& directory);

  bool Contains(const std::string& asset_id) const override;
  std::vector<std::string> GetAssets() const override;
  std::string GetAssetType(const std::string& asset_id) const override;
  std::vector<std::string> GetAssetFileTypes(const std::string& asset_id) const override;
  std::optional<std::string> LoadAssetTextFile(const std::string& asset_id,
                                               const std::string& file_type) const override;
  std::optional<Blob> LoadAssetBinaryFile(const std::string& asset_id, const std::string& file_type) const override;
  std::vector<std::string> GetAssetsWithType(const std::string& type) const override;

  bool CreateAsset(const std::string& asset_id, const std::string& type,
                   const std::vector<std::pair<std::string, std::variant<std::string, Blob>>>& files) override;

  bool SaveAssetFile(const std::string& asset_id, const std::string& file_type,
                     std::variant<std::string, Blob> content) override;

  void Rescan();
  inline std::string directory() const { return directory_; }

 protected:
  std::optional<std::string> GetAssetFilename(const std::string& asset_id, const std::string& file_type) const;

 private:
  std::filesystem::path directory_;

  struct AssetData {
    std::filesystem::path directory;
    std::string type;
    std::vector<std::string> file_types;
  };

  std::unordered_map<std::string, AssetData> assets_;
  std::unordered_multimap<std::string, std::string> assets_with_type_;
};

namespace detail {
void SetEngineAssetLibrary(std::unique_ptr<AssetLibrary> asset_library);
void SetApplicationAssetLibrary(std::unique_ptr<AssetLibrary> asset_library);
}  // namespace detail

template <typename T, typename... Args>
void CreateEngineAssetLibrary(Args... args) {
  static_assert(std::is_base_of<AssetLibrary, T>::value, "T must derive from AssetLibrary");
  detail::SetEngineAssetLibrary(std::make_unique<T>(std::forward<Args>(args)...));
}
void SetEngineAssetsDirectory(const std::string& directory);

template <typename T, typename... Args>
void CreateApplicationAssetLibrary(Args... args) {
  static_assert(std::is_base_of<AssetLibrary, T>::value, "T must derive from AssetLibrary");
  detail::SetApplicationAssetLibrary(std::make_unique<T>(std::forward<Args>(args)...));
}
void SetApplicationAssetsDirectory(const std::string& directory);

AssetLibrary* GetEngineAssetLibrary();
AssetLibrary* GetApplicationAssetLibrary();

}  // namespace ovis
