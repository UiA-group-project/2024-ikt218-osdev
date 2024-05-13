// From JamesM's kernel development tutorials.
#ifndef MONITOR_H
#define MONITOR_H

#include <libc/common.h>

// Updates the hardware cursor.
static void move_cursor();

// Scrolls the text on the screen up by one line.
static void scroll();

// Writes a single character out to the screen.
void monitor_put(char c);

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear();

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

// Outputs a hex number to the monitor.
void monitor_write_hex(u32int n);

// Outputs a decimal number to the monitor.
void monitor_write_dec(u32int n);

// Wrapper function for printf
void printf(char *format, ...);

// Test function for monitor
void test_monitor();

#endif // MONITOR_H