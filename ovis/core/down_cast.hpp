#pragma once

#include <memory>

#define OVIS_USE_DYNAMIC_CAST 0

namespace ovis {

template <typename DestType, typename SourceType>
inline DestType down_cast(SourceType pointer) {
#if OVIS_USE_DYNAMIC_CAST
  return dynamic_cast<DestType>(pointer);
#else
  return static_cast<DestType>(pointer);
#endif
}

template <typename DestType, typename SourceType>
inline std::shared_ptr<DestType> down_cast(const std::shared_ptr<SourceType>& pointer) {
#if OVIS_USE_DYNAMIC_CAST
  return std::dynamic_pointer_cast<DestType>(pointer);
#else
  return std::static_pointer_cast<DestType>(pointer);
#endif
}

template <typename DestType, typename SourceType>
inline std::shared_ptr<DestType> down_cast(std::shared_ptr<SourceType>&& pointer) {
#if OVIS_USE_DYNAMIC_CAST
  return std::dynamic_pointer_cast<DestType>(std::move(pointer));
#else
  return std::static_pointer_cast<DestType>(std::move(pointer));
#endif
}

}  // namespace ovis
