# C-String

String of byte sized chars. A string implementation in C that supports [**Small String Optimization**](https://blogs.msmvps.com/gdicanio/2016/11/17/the-small-string-optimization/). This implementation differs by allowing small strings of up to a length of **32 characters** including the null terminator. Standard SSO implementations do not support this. The string (excluding heap string data) has a size of 32 bytes. 

String is able to do the following:

- Small String Optimization. Any string of size less 33 characters including the null terminator will be allocated within the string data itself, not in the data pointer.
- Support long strings (33+ chars in the heap).
- 32 byte size is a power of 2, thus (hopefully) fitting in cache lines cleanly.
- Append another string.
- Concatenate two strings into a new one.
- Get as c-style string

<h2>Notable Improvements</h2>

Final byte in structure, flags, is union aligned with the final char in the SSO. When the flag byte is set to 0, it can simultaneously function as a null terminator for 31 length strings.

> XX X XXXXX
> 
> First 5 bits in flags is the offset length from 31.
>
> 6th bit is flag. 0: SSO. 1: Long String.
>
> Last 2 bits are unused.
