#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <ovis/core/profiling.hpp>

namespace ovis {
namespace {
std::string GetFilenameFriendlyCurrentTimeString() {
  std::time_t current_time = std::time(nullptr);
  std::stringstream stream;
  stream << std::put_time(std::localtime(&current_time), "ovis-profiling_%Y-%m-%d_%H-%M-%S_%A_%d_%B_%H-%M.log");
  return stream.str();
}

ProfilingLog default_profiling_log{GetFilenameFriendlyCurrentTimeString()};
}  // namespace

ProfilingLog::ProfilingLog(const std::string& filename, char delimiter)
    : profiling_log_(filename), delimiter_{delimiter} {
  profiling_log_ << "Frame" << delimiter_ << "Profiler ID" << delimiter_ << "Value" << delimiter_ << "Unit"
                 << std::endl;
}

ProfilingLog::~ProfilingLog() {
  SDL_assert(profilers_.size() == 0);
}

ProfilingLog* ProfilingLog::default_log() {
  return &ovis::default_profiling_log;
}

void ProfilingLog::AddProfiler(Profiler* profiler_to_add) {
  for (auto profiler : profilers_) {
    SDL_assert(profiler->id() != profiler_to_add->id());
  }
  profilers_.push_back(profiler_to_add);
}
void ProfilingLog::RemoveProfiler(Profiler* profiler) {
  auto profiler_it = std::find(profilers_.begin(), profilers_.end(), profiler);
  SDL_assert(profiler_it != profilers_.end());
  profilers_.erase(profiler_it);
}

void ProfilingLog::Write(std::uint64_t frame_id, const std::string& profiler_id, double measurement_value,
                         const std::string& unit) {
  profiling_log_ << frame_id << delimiter_ << profiler_id << delimiter_ << measurement_value << delimiter_ << unit
                 << '\n';
  profiling_log_.flush();
}

void ProfilingLog::Write(const std::string& profiler_id, double measurement_value, const std::string& unit) {
  Write(current_frame_id_, profiler_id, measurement_value, unit);
}

Profiler::Profiler(ProfilingLog* profiling_log, const std::string& id, const std::string& unit,
                   size_t measurement_buffer_size)
    : id_(id), unit_(unit), profiling_log_(profiling_log) {
  profiling_log_->AddProfiler(this);
  measurement_points_.reserve(measurement_buffer_size);
}

Profiler::~Profiler() {
  profiling_log_->RemoveProfiler(this);
}

void Profiler::AddMeasurement(std::uint64_t frame_id, double measurement) {
  profiling_log_->Write(frame_id, id_, measurement, unit_);
  AppendMeasurement(measurement);
}

void Profiler::AddMeasurement(double measurement) {
  profiling_log_->Write(id_, measurement, unit_);
  AppendMeasurement(measurement);
}

void Profiler::ExtractLastMeasurements(double* buffer, size_t buffer_size, size_t* extracted_value_count) {
  //     1st half    2nd half
  //  _______|_____  ___|__
  // |             ||      |
  // [t10][t11][t12][t8][t9]
  //                  |
  //        index_of_oldest_measurement_
  // -> first: extract second half:
  const size_t number_of_elements_in_second_half = measurement_points_.size() - index_of_oldest_measurement_;
  const size_t number_of_elements_to_extract_from_second_half =
      std::min(number_of_elements_in_second_half, buffer_size);
  memcpy(buffer, measurement_points_.data() + index_of_oldest_measurement_,
         number_of_elements_in_second_half * sizeof(double));

  // -> second: extract first half
  const size_t remaining_buffer_size = buffer_size - number_of_elements_to_extract_from_second_half;
  const size_t number_of_elements_in_first_half = index_of_oldest_measurement_;
  const size_t number_of_elements_to_extract_from_first_half =
      std::min(number_of_elements_in_first_half, remaining_buffer_size);
  memcpy(buffer + number_of_elements_to_extract_from_second_half, measurement_points_.data(),
         number_of_elements_in_first_half * sizeof(double));

  if (extracted_value_count != nullptr) {
    *extracted_value_count =
        number_of_elements_to_extract_from_second_half + number_of_elements_to_extract_from_first_half;
  }
}

void Profiler::AppendMeasurement(double measurement) {
  if (measurement_points_.size() < measurement_points_.capacity()) {
    measurement_points_.push_back(measurement);
  } else {
    measurement_points_[index_of_oldest_measurement_] = measurement;
    ++index_of_oldest_measurement_;
    if (index_of_oldest_measurement_ == measurement_points_.size()) {
      index_of_oldest_measurement_ = 0;
    }
  }
}

CPUTimeProfiler::CPUTimeProfiler(const std::string& id)
    : Profiler(ProfilingLog::default_log(), "CPU::" + id, "ms"), measurement_started_(false) {}

}  // namespace ovis
