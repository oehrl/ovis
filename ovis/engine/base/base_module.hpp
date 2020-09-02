#pragma once

#include <imgui.h>

#include <ovis/engine/module.hpp>

namespace ovis {

class BaseModule final : public Module {
 public:
  BaseModule();
  ~BaseModule() override;

 private:
  ImGuiContext* context_;
};

}  // namespace ovis