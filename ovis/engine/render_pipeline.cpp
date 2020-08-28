#include <map>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>
#include <ovis/graphics/render_target_configuration.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/render_pipeline.hpp>

namespace ovis {

RenderPipeline::RenderPipeline(GraphicsContext* graphics_context, ResourceManager* resource_manager)
    : graphics_context_(graphics_context), resource_manager_(resource_manager), render_passes_sorted_(false) {
  SDL_assert(resource_manager != nullptr);
}

void RenderPipeline::AddRenderPass(const std::string& render_pass_id) {
  const auto render_pass_factory = RenderPass::factories()->find(render_pass_id);
  if (render_pass_factory == RenderPass::factories()->end()) {
    LogE("Cannot find render pass '{}'", render_pass_id);
    return;
  }

  if (render_passes_.count(render_pass_id) != 0) {
    LogE("Render pass '{}' already added", render_pass_id);
    return;
  }

  auto render_pass = render_pass_factory->second->CreateRenderPass(render_pass_id, this);
  if (render_pass == nullptr) {
    LogE("Failed to create render pass '{}'", render_pass_id);
    return;
  }

  auto insert_return_value = render_passes_.insert(std::make_pair(render_pass_id, std::move(render_pass)));
  SDL_assert(insert_return_value.second);
  insert_return_value.first->second->render_pipeline_ = this;
  insert_return_value.first->second->resource_manager_ = resource_manager_;
  if (graphics_context_ != nullptr) {
    insert_return_value.first->second->graphics_context_ = graphics_context_;
    insert_return_value.first->second->CreateResourcesWrapper();
  }
  render_passes_sorted_ = false;
}

void RenderPipeline::Render(Scene* scene) {
  if (!render_passes_sorted_) {
    SortRenderPasses();
  }
  for (auto* render_pass : render_pass_order_) {
    render_pass->RenderWrapper(scene);
  }
}

void RenderPipeline::DrawImGui() {
  for (auto* render_pass : render_pass_order_) {
    render_pass->DrawImGui();
  }
}

RenderTargetTexture2D* RenderPipeline::CreateRenderTarget2D(const std::string& id,
                                                            const RenderTargetTexture2DDescription& description) {
  if (render_targets_.count(id) == 0) {
    return static_cast<RenderTargetTexture2D*>(
        render_targets_
            .emplace(std::make_pair(id, std::make_unique<RenderTargetTexture2D>(graphics_context_, description)))
            .first->second.get());
  } else {
    LogE("Render target '{}' does already exist");
    return nullptr;
  }
}

RenderTarget* RenderPipeline::GetRenderTarget(const std::string& id) {
  const auto render_target = render_targets_.find(id);
  if (render_target == render_targets_.end()) {
    return nullptr;
  } else {
    return render_target->second.get();
  }
}

std::unique_ptr<RenderTargetConfiguration> RenderPipeline::CreateRenderTargetConfiguration(
    std::vector<std::string> color_render_target_ids, std::string depth_render_target_id) {
  RenderTargetConfigurationDescription render_target_config_desc;
  render_target_config_desc.color_attachments.reserve(color_render_target_ids.size());
  for (const auto& render_target_id : color_render_target_ids) {
    render_target_config_desc.color_attachments.push_back(GetRenderTarget(render_target_id));
    SDL_assert(render_target_config_desc.color_attachments.back() != nullptr);
  }
  if (depth_render_target_id.length() > 0) {
    render_target_config_desc.depth_attachment = GetRenderTarget(depth_render_target_id);
  }
  return std::make_unique<ovis::RenderTargetConfiguration>(graphics_context_, render_target_config_desc);
}

void RenderPipeline::SortRenderPasses() {
  // First depends on second beeing already rendered
  std::multimap<std::string, std::string> dependencies;
  std::set<std::string> render_passes_left_;

  for (const auto& name_renderer_pair : render_passes_) {
    render_passes_left_.insert(name_renderer_pair.first);

    for (auto render_before : name_renderer_pair.second->render_before_list_) {
      if (render_passes_.count(render_before) == 0) {
        LogW("Cannot render {0} before {1}, {1} not found!", name_renderer_pair.first, render_before);
      } else {
        dependencies.insert(std::make_pair(render_before, name_renderer_pair.first));
      }
    }

    for (auto render_after : name_renderer_pair.second->render_after_list_) {
      if (render_passes_.count(render_after) == 0) {
        LogW("Cannot render {0} after {1}, {1} not found!", name_renderer_pair.first, render_after);
      } else {
        dependencies.insert(std::make_pair(name_renderer_pair.first, render_after));
      }
    }
  }

  render_pass_order_.clear();
  render_pass_order_.reserve(render_passes_.size());
  while (render_passes_left_.size() > 0) {
    auto next = std::find_if(render_passes_left_.begin(), render_passes_left_.end(),
                             [&dependencies](const std::string& value) { return dependencies.count(value) == 0; });

    SDL_assert(next != render_passes_left_.end());

    SDL_assert(render_passes_.find(*next) != render_passes_.end());
    render_pass_order_.push_back(render_passes_[*next].get());
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

RenderPass* RenderPipeline::GetRenderPassInternal(const std::string& render_pass_name) const {
  auto render_pass = render_passes_.find(render_pass_name);
  if (render_pass == render_passes_.end()) {
    return nullptr;
  } else {
    return render_pass->second.get();
  }
}

}  // namespace ovis