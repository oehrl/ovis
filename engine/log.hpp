#pragma once

#include <iostream>
#include <utility>

enum class LogLevel
{
    VERBOSE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR
};


namespace details
{
    template <typename... T>
    struct LogHelper;
    
    template <>
    struct LogHelper<>
    {
        static void Log()
        {
            std::cout << std::endl;
        }
    };
    
    template <typename T, typename... U>
    struct LogHelper<T, U...>
    {
        static void Log(const T& arg, U&&... args)
        {
            std::cout << arg;
            LogHelper<U...>::Log(std::forward<U>(args)...);
        }
    };
    
    
    static const char LOG_LEVEL_CHARS[] =
    {
        'V',
        'D',
        'I',
        'W',
        'E'
    };
}

template <typename... T>
inline void Log(LogLevel level, T&&... args)
{
    const char log_level_char = details::LOG_LEVEL_CHARS[static_cast<int>(level)];
    details::LogHelper<char, const char*, T...>::Log(
        log_level_char,
        ": ",
        std::forward<T>(args)...
    );
}

template <typename... T> inline void LogV(T&&... args) { Log(LogLevel::VERBOSE, std::forward<T>(args)...); }
#ifdef DEBUG
template <typename... T> inline void LogD(T&&... args) { Log(LogLevel::DEBUG,   std::forward<T>(args)...); }
#else
template <typename... T> inline void LogD(T&&...     ) { }
#endif
template <typename... T> inline void LogI(T&&... args) { Log(LogLevel::INFO,    std::forward<T>(args)...); }
template <typename... T> inline void LogW(T&&... args) { Log(LogLevel::WARNING, std::forward<T>(args)...); }
template <typename... T> inline void LogE(T&&... args) { Log(LogLevel::ERROR,   std::forward<T>(args)...); }
