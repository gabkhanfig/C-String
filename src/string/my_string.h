#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

#define MAX_SMALL_STRING_LENGTH		31
#define STRING_FLAG_SMALL_SIZE_BITS 0b00011111
#define STRING_FLAG_IS_LONG			0b00100000
#define STRING_FLAG_UNUSED			0b11000000

/* Dynamic string with small string optimization for any string of strlen 32 or less.
Occupies 32 bytes on 64-bit compiler, which is a power of 2 to (hopefully) place multiple strings cleanly in cache lines. */
typedef struct string 
{
    union 
    {
        struct 
        {
            char* data;
            size_t length;
            size_t capacity;
            int _padding_0;
            short _padding_1;
            char _padding_2;
            unsigned char flags;
        };
        struct 
        {
            char chars[MAX_SMALL_STRING_LENGTH + 1];
        };
    };
} string;



extern void string_set_length(string* str, size_t new_length);
extern const char* string_cstr(string* str);
extern string string_construct_char(const char* chars);
extern string string_construct_copy(string* other);
extern void string_destruct(string* str);
extern unsigned char string_is_small(unsigned char flags);
extern string string_concatenate(string* left, string* right);
extern void string_append(string* str, string* other);



#endif