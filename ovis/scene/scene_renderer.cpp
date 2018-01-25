#include <SDL_assert.h>
#include <ovis/core/log.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_renderer.hpp>

namespace ovis {

SceneRenderer::SceneRenderer(Scene* scene, const std::string& name)
    : m_scene(scene), m_name(name) {
  SDL_assert(scene != nullptr);
  scene->AddRenderer(this);
  LogD("Renderer '", name, "' added to scene '", scene->name(), "'");
}

SceneRenderer::~SceneRenderer() {
  scene()->RemoveRenderer(this);
  LogD("Renderer '", name(), "' removed from scene '", scene()->name(), "'");
}

void SceneRenderer::RenderBefore(const std::string& renderer_name) {
  m_render_before_list.insert(renderer_name);
  m_scene->m_renderers_sorted = false;
}

void SceneRenderer::RenderAfter(const std::string& renderer_name) {
  m_render_after_list.insert(renderer_name);
  m_scene->m_renderers_sorted = false;
}

}  // namespace ovis
