#pragma once

#include <cstdint>
#include <ovis/graphics/gl.hpp>

namespace ovis {

enum class SourceBlendFactor : std::uint16_t {
  ZERO                        = GL_ZERO,
  ONE                         = GL_ONE,
  SOURE_COLOR                 = GL_SRC_COLOR,
  ONE_MINUS_SOURCE_COLOR      = GL_ONE_MINUS_SRC_COLOR,
  DESTINATION_COLOR           = GL_DST_COLOR,
  ONE_MINUS_DESTINATION_COLOR = GL_ONE_MINUS_DST_COLOR,
  SOURCE_ALPHA                = GL_SRC_ALPHA,
  ONE_MINUS_SOURCE_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
  DESTINATION_ALPHA           = GL_DST_ALPHA,
  ONE_MINUS_DESTINATION_ALPHA = GL_ONE_MINUS_DST_ALPHA,
  CONSTANT_COLOR              = GL_CONSTANT_COLOR,
  ONE_MINUS_CONSTANT_COLOR    = GL_ONE_MINUS_CONSTANT_COLOR,
  CONSTANT_ALPHA              = GL_CONSTANT_ALPHA,
  ONE_MINUS_CONSTANT_ALPHA    = GL_ONE_MINUS_CONSTANT_ALPHA,
  SOURCE_ALPHA_SATURATE       = GL_SRC_ALPHA_SATURATE,
};

enum class DestinationBlendFactor : std::uint16_t {
  ZERO                        = GL_ZERO,
  ONE                         = GL_ONE,
  SOURE_COLOR                 = GL_SRC_COLOR,
  ONE_MINUS_SOURCE_COLOR      = GL_ONE_MINUS_SRC_COLOR,
  DESTINATION_COLOR           = GL_DST_COLOR,
  ONE_MINUS_DESTINATION_COLOR = GL_ONE_MINUS_DST_COLOR,
  SOURCE_ALPHA                = GL_SRC_ALPHA,
  ONE_MINUS_SOURCE_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
  DESTINATION_ALPHA           = GL_DST_ALPHA,
  ONE_MINUS_DESTINATION_ALPHA = GL_ONE_MINUS_DST_ALPHA,
  CONSTANT_COLOR              = GL_CONSTANT_COLOR,
  ONE_MINUS_CONSTANT_COLOR    = GL_ONE_MINUS_CONSTANT_COLOR,
  CONSTANT_ALPHA              = GL_CONSTANT_ALPHA,
  ONE_MINUS_CONSTANT_ALPHA    = GL_ONE_MINUS_CONSTANT_ALPHA,
};

enum class BlendFunction : std::uint16_t {
  ADD              = GL_FUNC_ADD,
  SUBTRACT         = GL_FUNC_SUBTRACT,
  REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
};

struct BlendState {
  SourceBlendFactor source_color_factor = SourceBlendFactor::ONE;
  SourceBlendFactor source_alpha_factor = SourceBlendFactor::ONE;

  DestinationBlendFactor destination_color_factor =
      DestinationBlendFactor::ZERO;
  DestinationBlendFactor destination_alpha_factor =
      DestinationBlendFactor::ZERO;

  BlendFunction color_function = BlendFunction::ADD;
  BlendFunction alpha_function = BlendFunction::ADD;

  float constant_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

  bool enabled = false;
};
static_assert(sizeof(BlendState) == 32, "Invalid Padding");

}  // namespace ovis