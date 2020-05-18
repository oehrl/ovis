#pragma once

#include <iostream>
#include <ostream>
#include <utility>

#include <fmt/format.h>

namespace ovis {

enum class LogLevel { VERBOSE = 0, DEBUG, INFO, WARNING, ERROR };

namespace detail {

static const char LOG_LEVEL_CHARS[] = {'V', 'D', 'I', 'W', 'E'};

}  // namespace detail

template <typename... T>
inline void Log(LogLevel level, std::string_view format_string, T&&... args) {
  const char log_level_char = detail::LOG_LEVEL_CHARS[static_cast<int>(level)];
  fmt::format_to(std::ostream_iterator<char>(std::cout), "{}:", log_level_char);
  fmt::format_to(std::ostream_iterator<char>(std::cout), format_string, std::forward<T>(args)...);
  std::cout << '\n';
}

template <typename... T>
inline void LogV(std::string_view format_string, T&&... args) {
  Log(LogLevel::VERBOSE, format_string, std::forward<T>(args)...);
}

template <typename... T>
inline void LogD(std::string_view format_string, T&&... args) {
  Log(LogLevel::DEBUG, format_string, std::forward<T>(args)...);
}

template <typename... T>
inline void LogI(std::string_view format_string, T&&... args) {
  Log(LogLevel::INFO, format_string, std::forward<T>(args)...);
}

template <typename... T>
inline void LogW(std::string_view format_string, T&&... args) {
  Log(LogLevel::WARNING, format_string, std::forward<T>(args)...);
}

template <typename... T>
inline void LogE(std::string_view format_string, T&&... args) {
  Log(LogLevel::ERROR, format_string, std::forward<T>(args)...);
}

}  // namespace ovis
