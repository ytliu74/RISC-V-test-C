/*
 * arch types
 *
 * Copyright (C) 2017 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _ARCH_TYPES_H_
#define _ARCH_TYPES_H_

typedef unsigned long addr_t;
typedef unsigned int  word_t;

typedef long saddr_t;

/* Linux style typedefs */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef long unsigned int size_t;
typedef saddr_t ptrdiff_t;

#ifndef __cplusplus
typedef int bool;
#endif

#endif /* _ARCH_TYPES_H_ */
