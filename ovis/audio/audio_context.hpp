#pragma once

#include <memory>
#include <vector>

#include <sdl_mixer.h>

namespace ovis {

class AudioChannel;
class AudioChunk;

class AudioContext {
 public:
  AudioContext();
  ~AudioContext();

  AudioChannel* Play(AudioChunk* chunk);

 private:
  std::vector<std::unique_ptr<AudioChannel>> channels_;
};

}  // namespace ovis
