//basic printfunction 
    void write_string(int colour, const char *string) 
    {
        volatile char *video = (volatile char*)0xb8000;
        while(*string != 0) {
            *video++ = *string++;
            *video++ = colour;
        }

    }
    