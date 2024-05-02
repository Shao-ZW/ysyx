#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *p = s;
  while(*p)
    p++;
  return p - s;
}

char *strcpy(char *dst, const char *src) {
  char *p = dst;
  while(*src)
    *dst++ = *src++;
  *dst = '\0';
  return p;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *p = dst;
  size_t i = 0;
  while(i < n && *src) {
    *dst++ = *src++;
    i++;
  }
  *dst = '\0';
  return p;
}

char *strcat(char *dst, const char *src) {
  char *p = dst;
  while(*dst)
    dst++;
  while(*src)
    *dst++ = *src++;
  *dst = '\0';
  return p;
}

int strcmp(const char *s1, const char *s2) {
  while(*s1 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while(n && *s1 && *s1 == *s2) {
    s1++;
    s2++;
    n--;
  }
  if(n == 0)
    return 0;
  return *s1 - *s2;
}

void *memset(void *s, int c, size_t n) {
  char *d = (char *)s;
  for(int i = 0; i < n; ++i)
    d[i] = c;
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  char *d = (char *)dst;
  char *s = (char *)src;

  if(d < s) {
    while(n--)
      *d++ = *s++;
  }
  else {
    d += n;
    s += n;
    while(n--)
      *--d = *--s;
  }

  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  return memmove(out, in, n);
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char *p1 = (unsigned char *)s1;
  unsigned char *p2 = (unsigned char *)s2;

  while(n--) {
    if(*p1 != *p2)
      return *p1 - *p2;
    p1++;
    p2++;
  }
  return 0;
}

#endif






