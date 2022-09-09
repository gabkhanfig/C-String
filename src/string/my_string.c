#include <string.h>
#include <stdio.h>
#include "my_string.h"

void string_flag_to_small(string* str) 
{
    str->flags = str->flags * (STRING_FLAG_UNUSED + STRING_FLAG_SMALL_SIZE_BITS);
}

void string_flag_to_long(string* str) 
{
    str->flags = str->flags | STRING_FLAG_IS_LONG;
}

void string_set_length_small(string* str, size_t new_length) 
{
    str->flags = (MAX_SMALL_STRING_LENGTH - new_length) + (str->flags & STRING_FLAG_UNUSED);
}

void string_set_length_long(string* str, size_t new_length) 
{
    str->length = new_length;
}

unsigned char string_is_small(unsigned char flags) 
{
    return (flags & STRING_FLAG_IS_LONG) == 0;
}

size_t string_small_length(unsigned char flags) {
    return MAX_SMALL_STRING_LENGTH - (flags & STRING_FLAG_SMALL_SIZE_BITS);
}

size_t string_long_length(string* str) 
{
    return str->length;
}

void string_set_length(string* str, size_t new_length) 
{
    if(new_length <= MAX_SMALL_STRING_LENGTH) {
        string_set_length_small(str, new_length);
    }
    else {
        string_flag_to_long(str);
        string_set_length_long(str, new_length);
    }
}

const char* string_cstr(string* str) 
{
    if(string_is_small(str->flags)) {
        return str->chars;
    }
    else {
        return str->data;
    }
}

size_t string_length(string* str) 
{
    const unsigned char flags = str->flags;
    if(string_is_small(flags)) {
        return string_small_length(flags);
    }
    else {
        return string_long_length(str);
    }
}

string string_construct_char(const char* chars) 
{
    const size_t length = strlen(chars);
    string str;
    if(length <= MAX_SMALL_STRING_LENGTH) {
        string_set_length_small(&str, length);
        memcpy(str.chars, chars, length + 1);
    } 
    else {
        str.capacity = length + 1;
        str.data = (char*)malloc(str.capacity);
        memcpy(str.data, chars, str.capacity);
        str.length = length;
        str.flags = STRING_FLAG_IS_LONG;
    }
    return str;
}

string string_construct_copy(string* other) 
{
    const size_t length = string_length(other);
    string str;
    if(length <= MAX_SMALL_STRING_LENGTH) {
        string_set_length_small(&str, length);
        memcpy(str.chars, other->chars, length + 1);
    } 
    else {
        str.capacity = length + 1;
        str.data = malloc(str.capacity);
        memcpy(str.data, string_cstr(other), str.capacity);
        str.length = length;
        str.flags = STRING_FLAG_IS_LONG;
    }
    return str;
}

void string_destruct(string* str) 
{
    if(!string_is_small(str->flags)) {
        free(str->data);
    }
}

string string_concatenate(string* left, string* right) {
    const size_t length = string_length(left) + string_length(right);
    const size_t capacity = length + 1;
    const size_t left_length = string_length(left);

    const char* left_cstr = string_cstr(left);
    const char* right_cstr = string_cstr(right);

    string str;
    string_set_length(&str, length);
    if(string_is_small(str.flags)) {
        memcpy(str.chars, left_cstr, left_length);
        memcpy(&str.chars[left_length], right_cstr, string_small_length(right->flags) + 1);
    }
    else {
        printf("long path\n");
        str.data = (char*)malloc(capacity);
        memcpy(str.data, left_cstr, string_length(left));
        memcpy(&str.chars[left_length], right_cstr, string_length(right) + 1);
        str.capacity = capacity;
    }
    return str;
}

void string_append(string* str, string* other) 
{
    const size_t original_length = string_length(str);
    const size_t other_length = string_length(other);
    const size_t length = original_length + other_length;

    if(length <= MAX_SMALL_STRING_LENGTH) {
        printf("small path\n");
        memcpy(&str->chars[string_small_length(str->flags)], other->chars, string_small_length(other->flags) + 1);
        string_set_length_small(str, length);
    }
    else {
        size_t capacity = length + 1;

        if(string_is_small(str->flags) || capacity > str->capacity ) {
            capacity = (3 * capacity) >> 1; // x1.5
            char* new_data = (char*)malloc(capacity);
            memcpy(new_data, string_cstr(str), original_length + 1);
            if(string_is_small(str->flags)) {
                free(str->data);
                string_flag_to_long(str);
            }
            str->data = new_data;
            //str->length = original_length;
            str->capacity = capacity;
        }

        memcpy(&str->data[original_length], string_cstr(other), other_length + 1);
        string_set_length_long(str, length);
    }
}