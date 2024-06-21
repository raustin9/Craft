#include "platform.h"

#ifdef Q_PLATFORM_LINUX

namespace platform {

// Write a message to the console
void 
console_write(const std::string& msg) {
    fprintf(stdout, "%s", msg.c_str());
}

// Write an error to the console
void 
console_error(const std::string& msg) {
    fprintf(stderr, "%s", msg.c_str());
}

}

#endif /* Q_PLATFORM_LINUX */
