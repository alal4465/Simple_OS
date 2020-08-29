#include "libc.h"

void* memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

void* memcpy(void *dest, const void *src, size_t n)
{
    char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

size_t strlen(const char *s) {
    size_t i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

int atoi(char* str) 
{ 
    int res = 0; 
    for (int i = 0; str[i] != '\0';++i) 
        res = res * 10 + str[i] - '0'; 
  
    return res; 
}


