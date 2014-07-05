#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

#include <stdio.h>
#include <signal.h>

#define NO_OP ((void) 0)

constexpr const char *__trim(const char *orig) {
    return (*orig == '.' || *orig == '/' || *orig == ' ')
        ? __trim(orig + 1)
        : orig;
}

#ifdef NDEBUG

#define EXPECT(exp, message, args...) NO_OP
#define ASSERT(exp, message, args...) NO_OP

#else

inline void __failure_message(
        const char *assertion,
        const char *file,
        const char *function,
        const int line,
        const char *message,
        ...) {
    fprintf(stderr, "%s:%s:%d: '%s' ",
            file, function, line, assertion);
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
}

#define EXPECT(exp, message, args...)\
    if (!(exp)) {\
        __failure_message(#exp, __trim(__FILE__), __func__, __LINE__, message, ##args);\
    }

#define ASSERT(exp, message, args...)\
    if (!(exp)) {\
        __failure_message(#exp, __trim(__FILE__), __func__, __LINE__, message, ##args);\
        raise(SIGTRAP);\
    }

#endif

#endif  // SRC_UTIL_DEBUG_H_
