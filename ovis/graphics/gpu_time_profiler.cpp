#include <ovis/graphics/gpu_time_profiler.hpp>

namespace ovis {

GPUQuery::GPUQuery(GraphicsContext* context)
    : GraphicsResource(context), name_(0) {
  glGenQueries(1, &name_);
  SDL_assert(name_ != 0);
}

GPUQuery::~GPUQuery() {
  glDeleteQueries(1, &name_);
}

GPUElapsedTimeQuery::GPUElapsedTimeQuery(GraphicsContext* context)
    : GPUQuery(context) {}

void GPUElapsedTimeQuery::Begin() {
  SDL_assert(!started_);
  glBeginQuery(GL_TIME_ELAPSED, name_);
  started_ = true;
}
void GPUElapsedTimeQuery::End() {
  SDL_assert(started_);
  glEndQuery(GL_TIME_ELAPSED);
  started_ = false;
}

GPUTimeProfiler::GPUTimeProfiler(GraphicsContext* context,
                                 const std::string& id)
    : Profiler(ProfilingLog::default_log(), "GPU::" + id, "ms"), query_(context) {}

GPUTimeProfiler::~GPUTimeProfiler() {
  if (query_will_have_data_) {
    AddMeasurement(*query_.GetResult(true) / 1000000.0);
  }
}

bool GPUElapsedTimeQuery::IsResultAvailable() {
  GLint result;
  glGetQueryObjectiv(name_, GL_QUERY_RESULT_AVAILABLE, &result);
  return result == GL_TRUE;
}

std::optional<std::uint64_t> GPUElapsedTimeQuery::GetResult(bool wait) {
  if (wait || IsResultAvailable()) {
    GLuint64 result;
    glGetQueryObjectui64v(name_, GL_QUERY_RESULT, &result);
    return result;
  } else {
    return {};
  }
}

}  // namespace ovis