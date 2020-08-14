#pragma once

#include <set>
#include <string>
#include <unordered_map>

#if OVIS_ENABLE_BUILT_IN_PROFILING
#include <ovis/core/profiling.hpp>

#include <ovis/graphics/gpu_time_profiler.hpp>
#endif

namespace ovis {

class RenderPipeline;
class GraphicsContext;
class ResourceManager;
class Scene;
class Module;

class RenderPass {
  friend class RenderPipeline;
  friend class Module;

 public:
  RenderPass(const std::string& name);
  virtual ~RenderPass() = default;

  inline RenderPipeline* render_pipeline() const { return render_pipeline_; }
  inline std::string name() const { return name_; }
  inline GraphicsContext* context() const { return graphics_context_; }
  inline ResourceManager* resource_manager() const { return resource_manager_; }

  virtual void CreateResources() {}
  virtual void ReleaseResources() {}
  virtual void Render(Scene* scene) = 0;

  virtual void DrawDebugUI() {}

  static std::vector<std::string> GetRegisteredRenderPasses();

 protected:
  void RenderBefore(const std::string& renderer_name);
  void RenderAfter(const std::string& renderer_name);

 private:
  static std::unordered_map<std::string, Module*>* render_pass_factories();

  RenderPipeline* render_pipeline_ = nullptr;
  GraphicsContext* graphics_context_ = nullptr;
  ResourceManager* resource_manager_ = nullptr;
  std::string name_;
  std::set<std::string> render_before_list_;
  std::set<std::string> render_after_list_;

#if OVIS_ENABLE_BUILT_IN_PROFILING
  CPUTimeProfiler cpu_render_profiler_;
  std::unique_ptr<GPUTimeProfiler> gpu_render_profiler_;
#endif

  inline void RenderWrapper(Scene* scene) {
#if OVIS_ENABLE_BUILT_IN_PROFILING
    cpu_render_profiler_.BeginMeasurement();
    gpu_render_profiler_->BeginMeasurement();
#endif
    Render(scene);
#if OVIS_ENABLE_BUILT_IN_PROFILING
    cpu_render_profiler_.EndMeasurement();
    gpu_render_profiler_->EndMeasurement();
#endif
  }

  inline void CreateResourcesWrapper() {
    gpu_render_profiler_ =
        std::make_unique<GPUTimeProfiler>(context(), name() + "::Render");
    CreateResources();
  }
  inline void ReleaseResourcesWrapper() {
    gpu_render_profiler_.reset();
    ReleaseResources();
  }
};

}  // namespace ovis
