
#include <ovis/rendering2d/rendering2d_module.hpp>
#include <ovis/rendering2d/sprite_component.hpp>
#include <ovis/rendering2d/sprite_renderer.hpp>

#include <ovis/core/log.hpp>

namespace ovis {

Rendering2DModule::Rendering2DModule() : Module("Rendering2D") {
  RegisterRenderPass("SpriteRenderer", [](Viewport*) { return std::make_unique<SpriteRenderer>(); });
  RegisterSceneObjectComponent<SpriteComponent>("Sprite",
                                                [](SceneObject*) { return std::make_unique<SpriteComponent>(); });
}

Rendering2DModule::~Rendering2DModule() {}

}  // namespace ovis
