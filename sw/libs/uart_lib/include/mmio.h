/*
 * Architecture specific Memory Mapped I/O functions
 * and defines
 *
 * Copyright (C) 2014 Intel corp. All rights reserved.
 *
 * Author: Stepan Karpenko <stepan.karpenko@intel.com>
 *
 */
#ifndef _ARCH_MMIO_H_
#define _ARCH_MMIO_H_

#include "types.h"
#include "compiler.h"


/* Memory barriers */
#define __arch_mmiorb() barrier()
#define __arch_mmiowb() barrier()


/* Macro definitions for MMIO operations */
#define __build_read_operation(type, suffix) \
	static inline type __arch_read##suffix(addr_t addr) \
		{ return *((volatile type*)addr); }

#define __build_write_operation(type, suffix) \
	static inline void __arch_write##suffix(type data, addr_t addr) \
		{ *((volatile type*)addr) = data; }

#define __build_mmio_read_operation(type, suffix) \
	static inline type __arch_mmio_read##suffix(addr_t addr) \
		{ type __ret =  *((volatile type*)addr); __arch_mmiorb(); return __ret; }

#define __build_mmio_write_operation(type, suffix) \
	static inline void __arch_mmio_write##suffix(type data, addr_t addr) \
		{ *((volatile type*)addr) = data; __arch_mmiowb(); }


/* Define read*()/write*() operations */
__build_read_operation(u8, b)
__build_write_operation(u8, b)
__build_read_operation(u16, w)
__build_write_operation(u16, w)
__build_read_operation(u32, l)
__build_write_operation(u32, l)
__build_read_operation(u64, q)
__build_write_operation(u64, q)

/* Define mmio_read*()/mmio_write*() operations */
__build_mmio_read_operation(u8, b)
__build_mmio_write_operation(u8, b)
__build_mmio_read_operation(u16, w)
__build_mmio_write_operation(u16, w)
__build_mmio_read_operation(u32, l)
__build_mmio_write_operation(u32, l)
__build_mmio_read_operation(u64, q)
__build_mmio_write_operation(u64, q)


/** Actual programming interface definition **/

/* Memory barriers */
#define mmiorb()	__arch_mmiorb()
#define mmiowb()	__arch_mmiowb()

/* Raw memory access  */
#define readb(a)	__arch_readb(a)
#define writeb(d,a)	__arch_writeb(d,a)
#define readw(a)	__arch_readw(a)
#define writew(d,a)	__arch_writew(d,a)
#define readl(a)	__arch_readl(a)
#define writel(d,a)	__arch_writel(d,a)
#define readq(a)	__arch_readq(a)
#define writeq(d,a)	__arch_writeq(d,a)

/* MMIO access */
#define mmio_readb(a)		__arch_mmio_readb(a)
#define mmio_writeb(d,a)	__arch_mmio_writeb(d,a)
#define mmio_readw(a)		__arch_mmio_readw(a)
#define mmio_writew(d,a)	__arch_mmio_writew(d,a)
#define mmio_readl(a)		__arch_mmio_readl(a)
#define mmio_writel(d,a)	__arch_mmio_writel(d,a)
#define mmio_readq(a)		__arch_mmio_readq(a)
#define mmio_writeq(d,a)	__arch_mmio_writeq(d,a)


#endif /* _ARCH_MMIO_H_ */
