#include <cassert>

#include <ovis/core/log.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

Module::Module(const std::string& name) : name_(name) {}

Module::~Module() {}

void Module::RegisterRenderPass(const std::string& id, const RenderPassFactoryFunction& factory_function) {
  if (!render_pass_factory_functions()->insert(std::make_pair(id, factory_function)).second) {
    LogE("The render pass '{}' was already registered", id);
  }
}

void Module::RegisterSceneController(const std::string& id, const SceneControllerFactoryFunction& factory_function) {
  if (!scene_controller_factory_functions()->insert(std::make_pair(id, factory_function)).second) {
    LogE("The scene controller '{}' was already registered", id);
  }
}

void Module::RegisterSceneObjectComponent(const std::string& id,
                                          const SceneObjectComponentFactoryFunction& factory_function) {
  if (!scene_object_component_factory_functions()->insert(std::make_pair(id, factory_function)).second) {
    LogE("The scene object component '{}' was already registered", id);
  }
}

void Module::RegisterFileLoader(const std::string& extension,
                                const ResourceLoadingFunction& resource_loading_function) {}

void Module::DeregisterRenderPass(const std::string& id) {
  const auto factory = render_pass_factory_functions()->find(id);
  if (factory == render_pass_factory_functions()->end()) {
    LogE("The render pass '{}' was not registered!", id);
  } else {
    render_pass_factory_functions()->erase(factory);
  }
}

void Module::DeregisterSceneController(const std::string& id) {
  const auto factory = scene_controller_factory_functions()->find(id);
  if (factory == scene_controller_factory_functions()->end()) {
    LogE("The scene controller '{}' was not registered!", id);
  } else {
    scene_controller_factory_functions()->erase(factory);
  }
}

void Module::DeregisterSceneObjectComponent(const std::string& id) {
  const auto factory = scene_object_component_factory_functions()->find(id);
  if (factory == scene_object_component_factory_functions()->end()) {
    LogE("The scene object component '{}' was not registered!", id);
  } else {
    scene_object_component_factory_functions()->erase(factory);
  }
}

std::map<std::string, Module::RenderPassFactoryFunction>* Module::render_pass_factory_functions() {
  static auto factory_functions = new std::map<std::string, RenderPassFactoryFunction>();
  return factory_functions;
}

std::map<std::string, Module::SceneControllerFactoryFunction>* Module::scene_controller_factory_functions() {
  static auto factory_functions = new std::map<std::string, SceneControllerFactoryFunction>();
  return factory_functions;
}

std::map<std::string, Module::SceneObjectComponentFactoryFunction>* Module::scene_object_component_factory_functions() {
  static auto factory_functions = new std::map<std::string, SceneObjectComponentFactoryFunction>();
  return factory_functions;
}

// void Module::RegisterRenderPass(const std::string& id) {
//   if (!RenderPass::factories()->insert(std::make_pair(id, this)).second) {
//     LogE("The render pass '{}' was already registered", id);
//   }
// }

// void Module::RegisterSceneController(const std::string& id) {
//   if (!SceneController::factories()->insert(std::make_pair(id, this)).second) {
//     LogE("The scene controller '{}' was already registered", id);
//   }
// }

// void Module::RegisterSceneObjectComponent(const std::string& id) {
//   if (!SceneObjectComponent::factories()->insert(std::make_pair(id, this)).second) {
//     LogE("The scene object component '{}' was already registered", id);
//   }
// }

}  // namespace ovis