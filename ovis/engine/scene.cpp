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

Scene::Scene() : m_is_paused(true) {}

Scene::~Scene() {}

void Scene::AddController(const std::string& scene_controller_id) {
  const auto controller_factory =
      SceneController::scene_controller_factories()->find(scene_controller_id);
  if (controller_factory ==
      SceneController::scene_controller_factories()->end()) {
    LogE("Cannot find scene controller '{}'", scene_controller_id);
    return;
  }

  if (controllers_.count(scene_controller_id) != 0) {
    LogE("Scene controller '{}' already added", scene_controller_id);
    return;
  }

  auto controller = controller_factory->second->CreateSceneController(
      scene_controller_id, this);
  if (controller == nullptr) {
    LogE("Failed to create scene controller '{}'", scene_controller_id);
    return;
  }

  auto insert_return_value = controllers_.insert(
      std::make_pair(scene_controller_id, std::move(controller)));
  SDL_assert(insert_return_value.second);
  insert_return_value.first->second->m_scene = this;
  controllers_sorted_ = false;
}

void Scene::RemoveController(const std::string& id) {
  // TODO: implement
  SDL_assert(false && "RemoveSceneController not implemented");
  LogE("RemoveSceneController not implemented");
  controllers_sorted_ = false;
}

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

void Scene::SortControllers() {
  // First depends on second beeing already rendered
  std::multimap<std::string, std::string> dependencies;
  std::set<std::string> controllers_left_;

  for (const auto& name_controller_pair : controllers_) {
    controllers_left_.insert(name_controller_pair.first);

    for (auto update_before : name_controller_pair.second->update_before_list_) {
      if (controllers_.count(update_before) == 0) {
        LogW("Cannot update {0} before {1}, {1} not found!",
             name_controller_pair.first, update_before);
      } else {
        dependencies.insert(
            std::make_pair(update_before, name_controller_pair.first));
      }
    }

    for (auto update_after : name_controller_pair.second->update_after_list_) {
      if (controllers_.count(update_after) == 0) {
        LogW("Cannot update {0} after {1}, {1} not found!",
             name_controller_pair.first, update_after);
      } else {
        dependencies.insert(
            std::make_pair(name_controller_pair.first, update_after));
      }
    }
  }

  LogV("Sorting controllers:");
  controller_order_.clear();
  controller_order_.reserve(controllers_.size());
  while (controllers_left_.size() > 0) {
    auto next =
        std::find_if(controllers_left_.begin(), controllers_left_.end(),
                     [&dependencies](const std::string& value) {
                       return dependencies.count(value) == 0;
                     });

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

SceneController* Scene::GetControllerInternal(
    const std::string& controller_name) const {
  auto controller = controllers_.find(controller_name);
  if (controller == controllers_.end()) {
    return nullptr;
  } else {
    return controller->second.get();
  }
}

}  // namespace ovis
