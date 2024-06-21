#include "platform.h"

#ifdef Q_PLATFORM_WINDOWS

namespace platform {

// Write a message to the console
void 
console_write(const std::string& msg) {
}

// Write an error to the console
void 
console_error(const std::string& msg) {
}

}

#endif /* Q_PLATFORM_WINDOWS */
