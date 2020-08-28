#pragma once

#include <cstdint>

#include <ovis/core/flags.hpp>
#include <ovis/graphics/gl.hpp>

namespace ovis {

enum class DepthBufferFunction : std::uint16_t {
  NEVER = GL_NEVER,
  LESS = GL_LESS,
  LESS_OR_EQUAL = GL_LEQUAL,
  EQUAL = GL_EQUAL,
  NOT_EQUAL = GL_NOTEQUAL,
  GREATER_OR_EQUAL = GL_GEQUAL,
  GREATER = GL_GREATER,
  ALWAYS = GL_ALWAYS,
};

struct DepthBufferState {
  DepthBufferFunction function = DepthBufferFunction::LESS;

  bool write_enabled = true;
  bool test_enabled = false;
};
static_assert(sizeof(DepthBufferState) == 4, "Invalid padding");

inline void ApplyDepthBufferState(DepthBufferState* current_state, DepthBufferState new_state) {
  if (current_state->test_enabled != new_state.test_enabled) {
    if (new_state.test_enabled) {
      glEnable(GL_DEPTH_TEST);
    } else {
      glDisable(GL_DEPTH_TEST);
    }
    current_state->test_enabled = new_state.test_enabled;
  }

  if (new_state.test_enabled) {
    if (current_state->write_enabled != new_state.write_enabled) {
      glDepthMask(new_state.write_enabled);
      current_state->write_enabled = new_state.write_enabled;
    }
    if (current_state->function != new_state.function) {
      glDepthFunc(static_cast<GLenum>(new_state.function));
      current_state->function = new_state.function;
    }
  }
}

}  // namespace ovis