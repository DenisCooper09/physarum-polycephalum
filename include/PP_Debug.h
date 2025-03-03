#pragma once

#if defined(PP_DEBUG_BUILD)
#   include <stdio.h>
#   include <stdlib.h>
#   define PP_INFO(format, ...) fprintf(stdout, "[INFO] %s:%i\t| " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#   define PP_ERROR(format, ...) fprintf(stderr, "[ERROR] %s:%i\t| " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#   define PP_FATAL(exit_code, format, ...) fprintf(stderr, "[ERROR] %s:%i\t| " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); exit(exit_code)
#elif defined(PP_RELEASE_BUILD)
#   define PP_INFO(format, ...)
#   define PP_ERROR(format, ...)
#   define PP_FATAL(exit_code, format, ...)
#else
#   error "Unknown build type."
#endif
