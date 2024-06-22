#pragma once
#include "platform/platform.h"
#include <format>
#include <string>

namespace compiler {

namespace core {

namespace logger {

constexpr const char* RED = "\x1b[31m";
constexpr const char* ORANGE = "\x1b[48:5:166m";
constexpr const char* BLUE = "\x1b[34m";
constexpr const char* MAGENTA = "\x1b[35m";
constexpr const char* CYAN = "\x1b[36m";
constexpr const char* WHITE = "\x1b[37m";
constexpr const char* BLACK = "\x1b[30m";
constexpr const char* YELLOW = "\x1b[33m";
constexpr const char* GREEN = "\x1b[32m";
constexpr const char* DEFAULT = "\x1b[0m";
constexpr char ENDL = '\n';

template <typename... Args> 
void Fatal(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = RED + std::string("[FATAL]") + msg + DEFAULT + ENDL;
    platform::console_error(msg);
}

template <typename... Args> 
void Error(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = ORANGE + std::string("[ERROR]") + msg + DEFAULT + ENDL;
    platform::console_error(msg);
}

template <typename... Args> 
void Warn(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = YELLOW + std::string("[WARN]") + msg + DEFAULT + ENDL;
    platform::console_write(msg);
}

template <typename... Args> 
void Debug(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = MAGENTA + std::string("[DEBUG]") + msg + DEFAULT + ENDL;
    platform::console_write(msg);
}

template <typename... Args> 
void Info(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = BLUE + std::string("[INFO]") + msg + DEFAULT + ENDL;
    platform::console_write(msg);
}

template <typename... Args> 
void Trace(const char* format, Args&& ...args) {
    std::string msg = std::vformat(format, std::make_format_args(args...));

    msg = CYAN + std::string("[TRACE]") + msg + DEFAULT + ENDL;
    platform::console_write(msg);
}

} // namespace logger
} // namespace core
} // namespace compiler
