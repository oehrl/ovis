#pragma once

#include <utility>

namespace ovis {

void Init();
template <typename T, typename... Args>
void LoadModule(Args... args) {
  new T(std::forward<Args>(args)...);
}
void Run();
void Quit();

}  // namespace ovis
