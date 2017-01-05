#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// defined in Makefile
// #define DEBUGGING

#define EXPAND_VA_ARGS(...) __VA_ARGS__

#define LOG_STREAM (stderr)

#define LOG_INFO(msg, ...) fprintf(LOG_STREAM, "%s:%d:%s(): " msg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#ifdef DEBUGGING
#define LOG_DEBUG(msg, ...) fprintf(LOG_STREAM, "%s:%d:%s(): " msg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#else
#define LOG_DEBUG(msg, ...) do {} while (0)
#endif

#endif
