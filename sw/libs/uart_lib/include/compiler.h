/*
 * Compiler specific defines
 *
 * Copyright (C) 2015 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_


#define _PACKED		__attribute__((packed))
#define _ALIGNED(x)	__attribute__((aligned(x)))
#define _SECTION(x)	__attribute__((section(x)))
#define _DATA		__attribute__((section(".data")))
#define _RODATA		__attribute__((section(".rodata")))
#define _WEAK		__attribute__((weak))


/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")


#endif /* _COMPILER_H_ */
