#pragma once

#include <optional>

#include <ovis/core/profiling.hpp>
#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

class GraphicsContext;

class GPUQuery : public GraphicsResource {
 public:
  GPUQuery(GraphicsContext* context);
  virtual ~GPUQuery();

 protected:
  GLuint name_;
};

class GPUElapsedTimeQuery : public GPUQuery {
 public:
  GPUElapsedTimeQuery(GraphicsContext* context);

  void Begin();
  void End();

  bool IsResultAvailable();
  std::optional<std::uint64_t> GetResult(bool wait = false);

 private:
  bool started_ = false;
};

class GPUTimeProfiler : public Profiler {
 public:
  GPUTimeProfiler(GraphicsContext* context, const std::string& id);
  virtual ~GPUTimeProfiler();

  inline void BeginMeasurement() {
    if (query_will_have_data_) {
      AddMeasurement(*query_.GetResult(true) / 1000000.0);
      query_will_have_data_ = false;
    }
    query_.Begin();
  }
  inline void EndMeasurement() {
    query_.End();
    query_will_have_data_ = true;
  }

 private:
  GPUElapsedTimeQuery query_;
  bool query_will_have_data_ = false;
};

}  // namespace ovis