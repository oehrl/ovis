//
// Created by Simon Oehrl on 05/05/16.
//

#include <algorithm>
#include <map>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>

#include <ovis/engine/module.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

Scene::Scene(Window* window) : m_is_paused(true), window_(window) {}

Scene::~Scene() {}

void Scene::AddController(const std::string& scene_controller_id) {
  const auto controller_factory =
      SceneController::scene_controller_factories()->find(scene_controller_id);
  if (controller_factory ==
      SceneController::scene_controller_factories()->end()) {
    LogE("Cannot find scene controller '{}'", scene_controller_id);
    return;
  }

  if (m_controllers.count(scene_controller_id) != 0) {
    LogE("Scene controller '{}' already added", scene_controller_id);
    return;
  }

  auto controller =
      controller_factory->second->CreateSceneController(scene_controller_id);
  if (controller == nullptr) {
    LogE("Failed to create scene controller '{}'", scene_controller_id);
    return;
  }

  auto insert_return_value = m_controllers.insert(
      std::make_pair(scene_controller_id, std::move(controller)));
  SDL_assert(insert_return_value.second);
  insert_return_value.first->second->m_scene = this;
}

void RemoveSceneController(const std::string& id) {}

SceneObject* Scene::CreateObject(const std::string& object_name) {
  auto result = created_objects_.insert(std::make_pair(
      object_name, std::make_unique<SceneObject>(this, object_name)));
  SDL_assert(result.second);
  return result.first->second.get();
}

void Scene::DeleteObject(const std::string& object_name) {
  SDL_assert(created_objects_.count(object_name) == 1);
  created_objects_.erase(object_name);
}

SceneObject* Scene::GetObject(const std::string& object_name) {
  auto object = objects_.find(object_name);
  return object->second;
}

void Scene::BeforeUpdate() {
  for (const auto& controller : m_controllers) {
    controller.second->BeforeUpdate();
  }
}

void Scene::AfterUpdate() {
  for (const auto& controller : m_controllers) {
    controller.second->AfterUpdate();
  }
}

void Scene::Update(std::chrono::microseconds delta_time) {
  for (const auto& controller : m_controllers) {
    controller.second->UpdateWrapper(delta_time);
  }
}

void Scene::Pause() {
  if (!m_is_paused) {
    m_is_paused = true;
  }
}

bool Scene::ProcessEvent(const SDL_Event& event) {
  if (BeforeEventProcessing(event)) {
    return true;
  }

  for (const auto& scene_controller : m_controllers) {
    if (scene_controller.second->ProcessEvent(event)) {
      return true;
    }
  }

  return AfterEventProcessing(event);
}

void Scene::Resume() {
  if (m_is_paused) {
    m_is_paused = false;
  }
}

void Scene::AddObject(SceneObject* object) {
  SDL_assert(objects_.find(object->name()) == objects_.end());
  objects_.insert(std::make_pair(object->name(), object));
}

void Scene::RemoveObject(SceneObject* object) {
  SDL_assert(objects_.find(object->name()) != objects_.end() &&
             objects_.find(object->name())->second == object);
  objects_.erase(objects_.find(object->name()));
}

SceneController* Scene::GetControllerInternal(
    const std::string& controller_name) const {
  auto controller = m_controllers.find(controller_name);
  if (controller == m_controllers.end()) {
    return nullptr;
  } else {
    return controller->second.get();
  }
}

}  // namespace ovis
