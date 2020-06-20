#pragma once

namespace ovis {

class SceneComponentBase {
 public:
  virtual ~SceneComponentBase() = default;
};

template <typename T>
class SceneComponent {
 public:
  template <typename... Args>
  SceneComponent(Args&... component_arguments)
      : component_(std::move<Args>(component_arguments)...) {}

  T* component() { return &component_; }
  const T* component() const { return &component_; }

 private:
  T component_;
};

}  // namespace ovis
