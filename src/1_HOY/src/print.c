#include <libc/stdbool.h>
#include <libc/stdarg.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/limits.h>
#include <libc/stddef.h>
#include <libc/stdint.h>

//basic printfunction 
    void write_string(int colour, const char *string) 
    {
        volatile char *video = (volatile char*)0xb8000;
        while(*string != 0) {
            *video++ = *string++;
            *video++ = colour;
        }

    }

// printfunction: https://wiki.osdev.org/Meaty_Skeleton#libc.2Fstdio.2Fprintf.c 
int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}