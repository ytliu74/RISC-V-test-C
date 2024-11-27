/*
 * String formatting routines
 *
 * Copyright (C) 2014 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _VSPRINTF_H_
#define _VSPRINTF_H_

#include "types.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *_parse_integer_fixup_radix(const char *s, unsigned int *base);
unsigned int _parse_integer(const char *s, unsigned int base, unsigned long long *p);
unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base);
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base);
long simple_strtol(const char *cp, char **endp, unsigned int base);
long long simple_strtoll(const char *cp, char **endp, unsigned int base);

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char *buf, size_t size, const char *fmt, ...);
int scnprintf(char *buf, size_t size, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

int vsscanf(const char *buf, const char *fmt, va_list args);
int sscanf(const char *buf, const char *fmt, ...);

#ifdef __cplusplus
} /* "C" */
#endif

#endif /* _VSPRINTF_H_ */
