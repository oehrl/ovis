//
// Created by Simon Oehrl on 05/05/16.
//

#include <algorithm>
#include <map>

#include <SDL2/SDL_assert.h>

#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

Scene::Scene(const std::string& name, const glm::uvec2& size,
             bool hide_previous)
    : m_name(name),
      size_(size),
      m_is_paused(true),
      m_hides_previous(hide_previous) {}

Scene::~Scene() {}

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

void Scene::Update(std::chrono::microseconds delta_time) {
  OnUpdate(delta_time);
  for (auto controller : m_controllers) {
    controller.second->UpdateWrapper(delta_time);
  }
}

// void Scene::Render(float width, float height) {
//   if (!m_renderers_sorted) {
//     SortRenderers();
//   }

//   const float aspect_ratio = width / height;
//   camera_.SetAspectRadio(aspect_ratio);

//   for (auto renderer : m_render_order) {
//     renderer->Render();
//   }
// }

void Scene::Pause() {
  if (!m_is_paused) {
    m_is_paused = true;
    OnPause();
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
    OnResume();
  }
}

void Scene::AddController(SceneController* controller) {
  SDL_assert(m_controllers.find(controller->name()) == m_controllers.end());
  m_controllers.insert(std::make_pair(controller->name(), controller));
}

void Scene::RemoveController(SceneController* controller) {
  SDL_assert(m_controllers.find(controller->name()) != m_controllers.end() &&
             m_controllers.find(controller->name())->second == controller);
  m_controllers.erase(m_controllers.find(controller->name()));
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
    return controller->second;
  }
}

void Scene::OnUpdate(std::chrono::microseconds /*delta_time*/) {}

void Scene::OnResume() {}

void Scene::OnPause() {}

}  // namespace ovis
