#pragma once

size_t strlen(const char* str);

// Adding size_t definition
#if !defined(__size_t_defined) // prevent multiple definition errors
#define __size_t_defined
typedef unsigned int size_t;  // or use 'unsigned long' if on a 64-bit system
#endif