#pragma once

#include <set>
#include <string>
#include <ovis/core/class.hpp>
#include <ovis/scene/scene.hpp>

namespace ovis {

class SceneRenderer {
  friend class Scene;
  MAKE_NON_COPY_OR_MOVABLE(SceneRenderer);

 public:
  SceneRenderer(Scene* scene, const std::string& name);
  virtual ~SceneRenderer();

  inline Scene* scene() const { return m_scene; }
  inline std::string name() const { return m_name; }
  inline GraphicsContext* context() const { return scene()->context(); }

  virtual void CreateResources() {}
  virtual void ReleaseResources() {}
  virtual void Render() = 0;

 protected:
  void RenderBefore(const std::string& renderer_name);
  void RenderAfter(const std::string& renderer_name);

 private:
  Scene* m_scene;
  std::string m_name;
  std::set<std::string> m_render_before_list;
  std::set<std::string> m_render_after_list;
};

}  // namespace ovis
