#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <cstdio>

#define LOG(msg) std::printf("%s:%d msg\n", __FILE__, __LINE__);

#define LOGF(msg, ...) std::printf("%s:%d " msg "\n", __FILE__, __LINE__, __VA_ARGS__);


#endif
