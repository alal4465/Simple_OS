#ifndef H_LIBC
#define H_LIBC

typedef unsigned int size_t;

void* memset(void *s, int c, size_t n);
void* memcpy(void *dest, const void *src, size_t n);

int strcmp(const char* s1, const char* s2);
size_t strlen(const char *s);

int atoi(char* str);

#endif /* H_LIBC */


