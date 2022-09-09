#include <stdio.h>
#include "string/my_string.h"

int main() {
    string str = string_construct_char("Hello world!");
    if(string_is_small(str.flags)) {
        printf("is a small string\n");
    }
    printf(string_cstr(&str));
    printf("\n\n");
    string str2 = string_construct_char("not too big");
    printf(string_cstr(&str2)); printf("\n");
    string combo = string_concatenate(&str, &str2);
    
    printf(string_cstr(&combo));
    printf("\n\n");

    string small1 = string_construct_char("what ");
    string small2 = string_construct_char("up");
    string_append(&small1, &small2);
    printf(string_cstr(&small1));

    printf("\n\n");

    string long1 = string_construct_char("an extremely super duper ultra long string ");
    string long2 = string_construct_copy(&long1);
   
    string_append(&long1, &long2);
    printf(string_cstr(&long1));
}