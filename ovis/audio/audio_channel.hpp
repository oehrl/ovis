#pragma once

namespace ovis {

class AudioChunk;

class AudioChannel {
 public:
  void Play(AudioChunk* chunk);
  void Pause();
  void Resume();
  void Stop();
  bool IsPlaying() const;
  AudioChunk* GetChunk() const;
};

}  // namespace ovis
