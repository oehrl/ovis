#include <ovis/audio/audio_context.hpp>

namespace ovis {

AudioContext::AudioContext() {
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
    throw std::runtime_error("Failed to initialize audio device");
  }
}

}  // namespace ovis
