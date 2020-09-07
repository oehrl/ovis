#pragma once

#include <ovis/engine/module.hpp>

namespace ovis {

class Rendering2DModule final : public Module {
 public:
  Rendering2DModule();
  ~Rendering2DModule() override;
};

}  // namespace ovis