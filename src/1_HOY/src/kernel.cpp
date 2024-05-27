extern "C"
{
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include <libc/stdio.h>
#include <libc/malloc.h>
#include <libc/memory.h>
#include <libc/monitor.h>
}

// Existing global operator new overloads
void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void *ptr) noexcept
{
    free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    free(ptr);
}

// Add sized-deallocation functions
void operator delete(void *ptr, size_t size) noexcept
{
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

extern "C" int kernel_main(void);
int kernel_main()
{
    const char *str = "Hello from kernel_main!\n";
    printf("%s", str);

    // Allocate some memory using the kernel memory manager
    // THIS IS PART OF THE ASSIGNMENT
    void *some_memory = malloc(12345);
    void *memory2 = malloc(54321);
    void *memory3 = malloc(13331);
    const char *memory4 = new char[1000]();

    // More code....

    while (true)
    {
        /* code */
    }

    return 0;
}
