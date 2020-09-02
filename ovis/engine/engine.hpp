#pragma once

#include <memory>
#include <typeindex>
#include <utility>

namespace ovis {

class Module;

namespace detail {
Module* AddModule(std::type_index type, std::unique_ptr<Module> module);
Module* GetModule(std::type_index type);
}  // namespace detail

void Init();
template <typename T, typename... Args>
T* LoadModule(Args... args) {
  return static_cast<T*>(detail::AddModule(typeid(T), std::make_unique<T>(std::forward<Args>(args)...)));
}
template <typename T>
T* GetModule() {
  return static_cast<T*>(detail::GetModule(typeid(T)));
}
void Run();
void Quit();

}  // namespace ovis
