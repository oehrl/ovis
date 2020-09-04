//
// Created by Simon Oehrl on 05/05/16.
//

#include <map>
#include <string>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>
#include <ovis/core/utf8.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

Scene::Scene() : m_is_paused(true) {}

Scene::~Scene() {}

void Scene::AddController(const std::string& scene_controller_id) {
  const auto controller_factory = Module::scene_controller_factory_functions()->find(scene_controller_id);
  if (controller_factory == Module::scene_controller_factory_functions()->end()) {
    LogE("Cannot find scene controller '{}'", scene_controller_id);
    return;
  }

  if (controllers_.count(scene_controller_id) != 0) {
    LogE("Scene controller '{}' already added", scene_controller_id);
    return;
  }

  auto controller = controller_factory->second(this);
  if (controller == nullptr) {
    LogE("Failed to create scene controller '{}'", scene_controller_id);
    return;
  }

  auto insert_return_value = controllers_.insert(std::make_pair(scene_controller_id, std::move(controller)));
  SDL_assert(insert_return_value.second);
  insert_return_value.first->second->m_scene = this;
  InvalidateControllerOrder();
}

void Scene::RemoveController(const std::string& id) {
  const auto scene_controller = controllers_.find(id);
  if (scene_controller == controllers_.end()) {
    LogE("The scene does not contain the controller '{}'", id);
  } else {
    controllers_.erase(scene_controller);
  }
  InvalidateControllerOrder();
}

SceneObject* Scene::CreateObject(const std::string& object_name) {
  auto result = objects_.insert(std::make_pair(object_name, std::make_unique<SceneObject>(this, object_name)));
  SDL_assert(result.second);
  return result.first->second.get();
}

SceneObject* Scene::CreateObject(const std::string& object_name, const json& serialized_object) {
  auto object = CreateObject(object_name);
  object->Deserialize(serialized_object);
  return object;
}

void Scene::DeleteObject(const std::string& object_name) {
  SDL_assert(objects_.count(object_name) == 1);
  objects_.erase(object_name);
}

SceneObject* Scene::GetObject(const std::string& object_name) {
  auto object = objects_.find(object_name);
  return object->second.get();
}

bool Scene::ContainsObject(const std::string& object_name) {
  return objects_.count(object_name) == 1;
}

void Scene::GetObjects(std::vector<SceneObject*>* scene_objects, bool sort_by_name) const {
  scene_objects->clear();
  scene_objects->reserve(objects_.size());
  for (auto& name_object_pair : objects_) {
    scene_objects->push_back(name_object_pair.second.get());
  }
  if (sort_by_name) {
    std::sort(scene_objects->begin(), scene_objects->end(),
              [](SceneObject* left, SceneObject* right) { return to_lower(left->name()) < to_lower(right->name()); });
  }
}

inline void Scene::GetSceneObjectsWithComponent(const std::string& component_id,
                                                std::vector<SceneObject*>* scene_objects) const {
  SDL_assert(scene_objects != nullptr);
  scene_objects->clear();

  for (auto& name_object_pair : objects_) {
    SceneObject* scene_object = name_object_pair.second.get();
    if (scene_object->HasComponent(component_id)) {
      scene_objects->push_back(scene_object);
    }
  }
}

void Scene::BeforeUpdate() {
  if (!controllers_sorted_) {
    SortControllers();
  }
  for (const auto& controller : controller_order_) {
    controller->BeforeUpdate();
  }
}

void Scene::AfterUpdate() {
  if (!controllers_sorted_) {
    SortControllers();
  }
  for (const auto& controller : controller_order_) {
    controller->AfterUpdate();
  }
}

void Scene::Update(std::chrono::microseconds delta_time) {
  if (!controllers_sorted_) {
    SortControllers();
  }
  for (const auto& controller : controller_order_) {
    controller->UpdateWrapper(delta_time);
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

  for (const auto& controller : controller_order_) {
    if (controller->ProcessEvent(event)) {
      return true;
    }
  }

  return AfterEventProcessing(event);
}

void Scene::DrawImGui() {
  for (const auto& controller : controller_order_) {
    controller->DrawImGui();
  }
}

json Scene::Serialize() const {
  return {};
}

void Scene::Deserialize(const json& serialized_object) {}

void Scene::Resume() {
  if (m_is_paused) {
    m_is_paused = false;
  }
}

void Scene::InvalidateControllerOrder() {
  controller_order_.clear();
  controllers_sorted_ = false;
}

void Scene::SortControllers() {
  // First depends on second beeing already rendered
  std::multimap<std::string, std::string> dependencies;
  std::set<std::string> controllers_left_;

  for (const auto& name_controller_pair : controllers_) {
    controllers_left_.insert(name_controller_pair.first);

    for (auto update_before : name_controller_pair.second->update_before_list_) {
      if (controllers_.count(update_before) == 0) {
        LogW("Cannot update {0} before {1}, {1} not found!", name_controller_pair.first, update_before);
      } else {
        dependencies.insert(std::make_pair(update_before, name_controller_pair.first));
      }
    }

    for (auto update_after : name_controller_pair.second->update_after_list_) {
      if (controllers_.count(update_after) == 0) {
        LogW("Cannot update {0} after {1}, {1} not found!", name_controller_pair.first, update_after);
      } else {
        dependencies.insert(std::make_pair(name_controller_pair.first, update_after));
      }
    }
  }

  LogV("Sorting controllers:");
  controller_order_.clear();
  controller_order_.reserve(controllers_.size());
  while (controllers_left_.size() > 0) {
    auto next = std::find_if(controllers_left_.begin(), controllers_left_.end(),
                             [&dependencies](const std::string& value) { return dependencies.count(value) == 0; });

    SDL_assert(next != controllers_left_.end());
    LogV(" {}", *next);

    SDL_assert(controllers_.find(*next) != controllers_.end());
    controller_order_.push_back(controllers_[*next].get());
    for (auto i = dependencies.begin(), e = dependencies.end(); i != e;) {
      if (i->second == *next) {
        i = dependencies.erase(i);
      } else {
        ++i;
      }
    }
    controllers_left_.erase(next);
  }

  LogV("Controllers sorted!");
  controllers_sorted_ = true;
}

SceneController* Scene::GetControllerInternal(const std::string& controller_name) const {
  auto controller = controllers_.find(controller_name);
  if (controller == controllers_.end()) {
    return nullptr;
  } else {
    return controller->second.get();
  }
}

}  // namespace ovis
