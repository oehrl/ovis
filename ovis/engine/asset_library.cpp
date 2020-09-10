#include <ovis/engine/asset_library.hpp>

namespace ovis {

AssetLibrary::AssetLibrary(const std::string& directory) : directory_(std::filesystem::absolute(directory)) {
  Refresh();
}

void AssetLibrary::Refresh() {
  asset_paths_.clear();
  asset_with_type_.clear();

  for (const auto& path : fs::recursive_directory_iterator(directory_)) {
    const auto filename = path.filename();
    const auto stem = path.stem();
    const auto extension = path.extension();

    if (extension == ".json") {
      const auto asset_extension = stem.extension();
      const std::string asset_type = asset_extension;
    }
  }
}

}  // namespace ovis
