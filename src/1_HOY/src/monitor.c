// function for writing to the screen
// based on jamesmolloy guide part 3 the screen
// https://github.com/cirosantilli/jamesmolloy-kernel-development-tutorials/blob/master/3_screen/monitor.c

#include "libc/monitor.h"

// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *)0xB8000;
// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;

// Updates the hardware cursor.
void move_cursor()
{
    // The screen is 80 characters wide...
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
void scroll()
{

    // Get a space character with the default colour attributes.
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if (cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0 * 80; i < 24 * 80; i++)
        {
            video_memory[i] = video_memory[i + 80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24 * 80; i < 25 * 80; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

// Writes a single character out to the screen.
void monitor_put(char c)
{
    // The background colour is black (0), the foreground is white (15).
    u8int backColour = 0;
    u8int foreColour = 15;

    // The attribute byte is made up of two nibbles - the lower being the
    // foreground colour, and the upper the background colour.
    u8int attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    u16int attribute = attributeByte << 8;
    u16int *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if (c >= ' ')
    {
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
    // Make an attribute byte for the default colours
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80 * 25; i++)
    {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
    int i = 0;
    while (c[i])
    {
        monitor_put(c[i++]);
    }
}

void monitor_write_hex(u32int n)
{
    // Convert the number to hexadecimal string
    char hex[9];
    int i = 0;
    while (n != 0)
    {
        int remainder = n % 16;
        if (remainder < 10)
        {
            hex[i] = remainder + '0';
        }
        else
        {
            hex[i] = remainder - 10 + 'A';
        }
        n = n / 16;
        i++;
    }
    hex[i] = '\0';

    // Reverse the hexadecimal string
    int len = i;
    char reversed_hex[9];
    for (i = 0; i < len; i++)
    {
        reversed_hex[i] = hex[len - i - 1];
    }
    reversed_hex[len] = '\0';

    // Write the hexadecimal string to the monitor
    monitor_write(reversed_hex);
}

void monitor_write_dec(u32int n)
{
    // Convert the number to decimal string
    char dec[11];
    int i = 0;
    if (n == 0)
    {
        dec[i++] = '0';
    }
    else
    {
        while (n != 0)
        {
            int remainder = n % 10;
            dec[i++] = remainder + '0';
            n = n / 10;
        }
    }
    dec[i] = '\0';
    // Reverse the decimal string
    int len = i;
    char reversed_dec[11];
    for (i = 0; i < len; i++)
    {
        reversed_dec[i] = dec[len - i - 1];
    }
    reversed_dec[len] = '\0';
    // Write the decimal string to the monitor
    monitor_write(reversed_dec);
}

void printf(char *format, ...)
{
    // Assuming that arguments are 4 bytes each and are placed on the stack
    // right to left, and that the stack grows downwards.

    // The address of the format parameter
    char **p = &format;

    // Move the pointer to the next argument on the stack
    p++;

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            // String
            case 's':
            {
                char *str = (char *)*p;
                monitor_write(str);
                p++;
                break;
            }
            // Decimal
            case 'd':
            {
                int num = *(int *)p;
                monitor_write_dec(num);
                p++;
                break;
            }
            // Hexadecimal
            case 'x':
            {
                int num = *(int *)p;
                monitor_write_hex(num);
                p++;
                break;
            }
            }
        }
        // Character
        else
        {
            monitor_put(*format);
        }
        format++;
    }
}

void test_monitor()
{
    monitor_clear();

    monitor_write("Testing monitor_write...\n");
    monitor_write_hex(0x123ABC);
    monitor_write("\n");
    monitor_write_dec(1234567890);
    monitor_write("\n");

    printf("Testing printf with a string: %s\n", "Hello, world!");
    printf("Testing printf with a decimal number: %d\n", 1234567890);
    printf("Testing printf with a hexadecimal number: %x\n", 0x123ABC);
    printf("Testing printf with multiple arguments: %s %d %x\n", "Hello, world!", 1234567890, 0x123ABC);
}