#include <map>

#include <SDL_assert.h>

#include <ovis/rendering/geometry_pass.hpp>
#include <ovis/rendering/render_pass.hpp>
#include <ovis/rendering/render_pipeline.hpp>

namespace ovis {

RenderPipeline::RenderPipeline(GraphicsContext* graphics_context,
                               ResourceManager* resource_manager)
    : graphics_context_(graphics_context),
      resource_manager_(resource_manager),
      render_passes_sorted_(false) {
  SDL_assert(resource_manager != nullptr);
}

RenderPipeline::~RenderPipeline() {
  while (render_passes_.size() > 0) {
    RemoveRenderPass(render_passes_.begin()->second);
  }
}

void RenderPipeline::AddRenderPass(RenderPass* render_pass) {
  SDL_assert(render_pass != nullptr);
  SDL_assert(render_pass->render_pipeline_ == nullptr);
  SDL_assert(render_passes_.find(render_pass->name()) == render_passes_.end());
  render_passes_.insert(std::make_pair(render_pass->name(), render_pass));
  render_pass->render_pipeline_ = this;
  render_pass->resource_manager_ = resource_manager_;
  if (graphics_context_ != nullptr) {
    render_pass->graphics_context_ = graphics_context_;
    render_pass->CreateResources();
  }
  render_passes_sorted_ = false;
}

void RenderPipeline::RemoveRenderPass(RenderPass* render_pass) {
  SDL_assert(render_passes_.find(render_pass->name()) != render_passes_.end() &&
             render_passes_.find(render_pass->name())->second == render_pass);
  SDL_assert(render_pass != nullptr);
  SDL_assert(render_pass->render_pipeline_ == this);
  if (graphics_context_ != nullptr) {
    render_pass->ReleaseResources();
    render_pass->graphics_context_ = nullptr;
  }
  render_pass->resource_manager_ = nullptr;
  render_pass->render_pipeline_ = nullptr;
  render_passes_.erase(render_passes_.find(render_pass->name()));
  render_passes_sorted_ = false;
}

void RenderPipeline::Render(Scene* scene) {
  if (!render_passes_sorted_) {
    SortRenderPasses();
  }
  for (auto* render_pass : render_pass_order_) {
    render_pass->Render();
  }
}

void RenderPipeline::SortRenderPasses() {
  // First depends on second beeing already rendered
  std::multimap<std::string, std::string> dependencies;
  std::set<std::string> render_passes_left_;

  for (auto name_renderer_pair : render_passes_) {
    render_passes_left_.insert(name_renderer_pair.first);

    for (auto render_before : name_renderer_pair.second->render_before_list_) {
      dependencies.insert(
          std::make_pair(render_before, name_renderer_pair.first));
    }

    for (auto render_after : name_renderer_pair.second->render_after_list_) {
      dependencies.insert(
          std::make_pair(name_renderer_pair.first, render_after));
    }
  }

  render_pass_order_.clear();
  render_pass_order_.reserve(render_passes_.size());
  while (render_passes_left_.size() > 0) {
    auto next =
        std::find_if(render_passes_left_.begin(), render_passes_left_.end(),
                     [&dependencies](const std::string& value) {
                       return dependencies.count(value) == 0;
                     });

    SDL_assert(next != render_passes_left_.end());

    render_pass_order_.push_back(render_passes_[*next]);
    for (auto i = dependencies.begin(), e = dependencies.end(); i != e;) {
      if (i->second == *next) {
        i = dependencies.erase(i);
      } else {
        ++i;
      }
    }
    render_passes_left_.erase(next);
  }

  render_passes_sorted_ = true;
}

RenderPass* RenderPipeline::GetRenderPassInternal(
    const std::string& render_pass_name) const {
  auto render_pass = render_passes_.find(render_pass_name);
  if (render_pass == render_passes_.end()) {
    return nullptr;
  } else {
    return render_pass->second;
  }
}

}  // namespace ovis