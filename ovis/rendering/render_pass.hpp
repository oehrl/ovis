#pragma once

#include <set>
#include <string>

namespace ovis {

class RenderPipeline;
class GraphicsContext;
class ResourceManager;

class RenderPass {
  friend class RenderPipeline;

 public:
  RenderPass(const std::string& name);
  virtual ~RenderPass() = default;

  inline RenderPipeline* render_pipeline() const { return render_pipeline_; }
  inline std::string name() const { return name_; }
  inline GraphicsContext* context() const { return graphics_context_; }
  inline ResourceManager* resource_manager() const { return resource_manager_; }

  virtual void CreateResources() {}
  virtual void ReleaseResources() {}
  virtual void Render() = 0;

 protected:
  void RenderBefore(const std::string& renderer_name);
  void RenderAfter(const std::string& renderer_name);

 private:
  RenderPipeline* render_pipeline_ = nullptr;
  GraphicsContext* graphics_context_ = nullptr;
  ResourceManager* resource_manager_ = nullptr;
  std::string name_;
  std::set<std::string> render_before_list_;
  std::set<std::string> render_after_list_;
};

}  // namespace ovis
