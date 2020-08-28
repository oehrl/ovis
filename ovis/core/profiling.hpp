#pragma once

#include <chrono>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

#include <SDL2/SDL_assert.h>

#include <ovis/core/class.hpp>

namespace ovis {

class Profiler;

class ProfilingLog final {
  MAKE_NON_COPYABLE(ProfilingLog);
  friend class Profiler;

 public:
  ProfilingLog(const std::string& output_filename, char delimiter = '\t');
  ~ProfilingLog();

  inline std::uint64_t current_frame() const { return current_frame_id_; }
  inline void AdvanceFrame() { ++current_frame_id_; }
  inline const std::vector<Profiler*>& profilers() const { return profilers_; }

  static ProfilingLog* default_log();

 private:
  std::uint64_t current_frame_id_;
  std::ofstream profiling_log_;
  std::vector<Profiler*> profilers_;
  char delimiter_;

  void AddProfiler(Profiler* profiler);
  void RemoveProfiler(Profiler* profiler);
  void Write(std::uint64_t frame_id, const std::string& profiler_id, double measurement_value, const std::string& unit);
  void Write(const std::string& profiler_id, double measurement_value, const std::string& unit);
};

class Profiler {
 public:
  Profiler(ProfilingLog* profiling_log, const std::string& id, const std::string& unit,
           size_t measurement_buffer_size = 50);

  virtual ~Profiler();

  inline const std::string& id() const { return id_; }
  inline const std::string& unit() const { return unit_; }

  void ExtractLastMeasurements(double* buffer, size_t buffer_size, size_t* extracted_value_count);

 protected:
  void AddMeasurement(std::uint64_t frame_id, double measurement);
  void AddMeasurement(double measurement);

 private:
  ProfilingLog* profiling_log_;
  std::string unit_;
  std::string id_;
  std::vector<double> measurement_points_;
  size_t index_of_oldest_measurement_ = 0;

  void AppendMeasurement(double measurement);
};

class CPUTimeProfiler : public Profiler {
  using clock = std::chrono::steady_clock;
  using target_duration = std::chrono::duration<double, std::milli>;

 public:
  CPUTimeProfiler(const std::string& id);

  inline void BeginMeasurement() {
    SDL_assert(measurement_started_ == false);
    begin_time_ = clock::now();
    measurement_started_ = true;
  }
  inline void EndMeasurement() {
    SDL_assert(measurement_started_ == true);
    target_duration duration = clock::now() - begin_time_;
    measurement_started_ = false;
    AddMeasurement(duration.count());
  }

 private:
  clock::time_point begin_time_;
  bool measurement_started_ = false;
};

}  // namespace ovis
