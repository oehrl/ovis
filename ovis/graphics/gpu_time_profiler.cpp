#include <ovis/graphics/gpu_time_profiler.hpp>

namespace ovis {

GPUQuery::GPUQuery(GraphicsContext* context) : GraphicsResource(context), name_(0) {
#if !OVIS_EMSCRIPTEN
  glGenQueries(1, &name_);
  SDL_assert(name_ != 0);
#endif
}

GPUQuery::~GPUQuery() {
#if !OVIS_EMSCRIPTEN
  glDeleteQueries(1, &name_);
#endif
}

GPUElapsedTimeQuery::GPUElapsedTimeQuery(GraphicsContext* context) : GPUQuery(context) {}

void GPUElapsedTimeQuery::Begin() {
  SDL_assert(!started_);
#if !OVIS_EMSCRIPTEN
  glBeginQuery(GL_TIME_ELAPSED, name_);
#endif
  started_ = true;
}
void GPUElapsedTimeQuery::End() {
  SDL_assert(started_);
#if !OVIS_EMSCRIPTEN
  glEndQuery(GL_TIME_ELAPSED);
#endif
  started_ = false;
}

GPUTimeProfiler::GPUTimeProfiler(GraphicsContext* context, const std::string& id)
    : Profiler(ProfilingLog::default_log(), "GPU::" + id, "ms"), query_(context) {}

GPUTimeProfiler::~GPUTimeProfiler() {
  if (query_will_have_data_) {
    AddMeasurement(*query_.GetResult(true) / 1000000.0);
  }
}

bool GPUElapsedTimeQuery::IsResultAvailable() {
#if !OVIS_EMSCRIPTEN
  GLint result;
  glGetQueryObjectiv(name_, GL_QUERY_RESULT_AVAILABLE, &result);
  return result == GL_TRUE;
#else
  return false;
#endif
}

std::optional<std::uint64_t> GPUElapsedTimeQuery::GetResult(bool wait) {
#if !OVIS_EMSCRIPTEN
  if (wait || IsResultAvailable()) {
    GLuint64 result;
    glGetQueryObjectui64v(name_, GL_QUERY_RESULT, &result);
    return result;
  } else {
    return {};
  }
#else
  return 0;
#endif
}

}  // namespace ovis