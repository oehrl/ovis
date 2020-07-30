#include <SDL2/SDL_assert.h>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_object.hpp>

namespace ovis {

SceneObject::SceneObject(Scene* scene, const std::string& name)
    : scene_(scene), name_(name) {
  SDL_assert(scene_ != nullptr);
  scene_->AddObject(this);
}

SceneObject::~SceneObject() {
  scene_->RemoveObject(this);
}

}  // namespace ovis
