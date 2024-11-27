/*
 * Taken from NewOS Project. BSD-like license.
 */
#ifndef _STRING_H_
#define _STRING_H_

#include "stddef.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *memchr(void const *buf, int c, size_t len);
int   memcmp(const void *cs, const void *ct, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, void const *src, size_t count);
void *memscan(void *addr, int c, size_t size);
void *memset(void *s, int c, size_t count);

char  *strcat(char *dest,  char const *src);
char  *strchr(const char *s, int c);
int    strcmp(char const *cs, char const *ct);
int    strcoll(const char *s1, const char *s2);
char  *strcpy(char *dest, char const *src);
size_t strlen(char const *s);
char  *strncat(char *dest, char const *src, size_t count);
int    strncmp(char const *cs, char const *ct, size_t count);
char  *strncpy(char *dest, char const *src, size_t count);
char  *strpbrk(char const *cs, char const *ct);
char  *strrchr(char const *s, int c);
size_t strspn(char const *s, char const *accept);
char  *strstr(char const *s1, char const *s2);
char  *strtok(char *s, char const *ct);
size_t strxfrm(char *dest, const char *src, size_t n);

/* non standard routines */
//void  *bcopy(void const *src, void *dest, size_t count);
void   bzero(void *dst, size_t count);
size_t strlcat(char *dst, char const *src, size_t s);
size_t strlcpy(char *dst, char const *src, size_t s);
int    strnicmp(char const *s1, char const *s2, size_t len);
int    strncasecmp(char const *s1, char const *s2, size_t len);
size_t strnlen(char const *s, size_t count);
char  *skip_spaces(const char *str);

#ifdef __cplusplus
} /* "C" */
#endif

#endif
