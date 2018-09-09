#ifndef IO_FUNCS_H
#define IO_FUNCS_H

#if PLATFORM==_UNIX
#include "platform/unix/io.h"
#elif PLATFORM==_WINDOWS
#include "platform/windows/io.h"
#else
#error "PLATFORM undefined"
#endif

#endif
