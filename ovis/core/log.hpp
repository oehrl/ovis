#pragma once

#include <iostream>
#include <ostream>
#include <utility>

namespace ovis {

enum class LogLevel { VERBOSE = 0, DEBUG, INFO, WARNING, ERROR };

namespace detail {

template <typename T>
void Log(std::ostream* stream, const T& arg) {
  *stream << arg << std::endl;
}

template <typename T, typename... U>
void Log(std::ostream* stream, const T& arg, U&&... args) {
  *stream << arg;
  Log<U...>(stream, std::forward<U>(args)...);
}

static const char LOG_LEVEL_CHARS[] = {'V', 'D', 'I', 'W', 'E'};
}  // namespace detail

template <typename... T>
inline void Log(LogLevel level, T&&... args) {
  const char log_level_char = detail::LOG_LEVEL_CHARS[static_cast<int>(level)];
  detail::Log<char, const char*, T...>(&std::cout, log_level_char, ": ",
                                       std::forward<T>(args)...);
}

template <typename... T>
inline void LogV(T&&... args) {
  Log(LogLevel::VERBOSE, std::forward<T>(args)...);
}
template <typename... T>
inline void LogD(T&&... args) {
  Log(LogLevel::DEBUG, std::forward<T>(args)...);
}
template <typename... T>
inline void LogI(T&&... args) {
  Log(LogLevel::INFO, std::forward<T>(args)...);
}
template <typename... T>
inline void LogW(T&&... args) {
  Log(LogLevel::WARNING, std::forward<T>(args)...);
}
template <typename... T>
inline void LogE(T&&... args) {
  Log(LogLevel::ERROR, std::forward<T>(args)...);
}

}  // namespace ovis
